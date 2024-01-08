#include "ImageFilter.h"

void ImageFilter::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::wstring& vertexPrefix, const std::wstring& pixelPrefix, int width, int height)
{
	// 화면이 꽉차도록 사각형을 하나 그려준다.
	MeshData meshData = GeometryGenerator::MakeSquare();
	m_mesh = std::make_shared<Mesh>();

	// vertex, index buffer 생성
	D3D11Utils::CreateVertexBuffer(device, meshData.vertices, m_mesh->vertexBuffer);
	D3D11Utils::CreateIndexBuffer(device, meshData.indices, m_mesh->indexBuffer);
	m_mesh->indexCount = UINT(meshData.indices.size());

	// InputLayout 설정
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Shader 생성 (이미지 필터를 위해 다양한 Shader응 사용할 수 있도록 지원)
	D3D11Utils::CreateVSAndInputLayout(device, vertexPrefix + L"VS.hlsl", inputElements, m_vertexShader, m_inputLayout);
	D3D11Utils::CreatePS(device, pixelPrefix + L"PS.hlsl", m_pixelShader);

	// Texture Sampler 설정
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// Image Filter에 일반적으로 사용하는 CLAMP 사용
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Texture Sampler 생성
	device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

	// Rasterization 방법 설정 (Image Filter는 효과만 주면 되므로 Depth buffer가 필요없다.)
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthClipEnable = false; // depth 버퍼 사용 X

	// 설정한 Rasterizer 생성
	device->CreateRasterizerState(&rastDesc, m_rasterizerState.GetAddressOf());

	// 경우의 따라 해상도가 높거나 낮아야하는 경우가 있어 viewport 설정
	// Runtime에서 바꿔줘도 된다.
	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(width);
	m_viewPort.Height = static_cast<float>(height);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	// Texture는 GPU의 메모리와 같은데 2차원 이미지를 다루는데 최적화되어 있다.
	D3D11_TEXTURE2D_DESC txtDesc;
	ZeroMemory(&txtDesc, sizeof(txtDesc));
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	// 고품질의 이미지 처리할 땐 용량이 큰 128 bits float을 사용하는 경우가 많다.
	txtDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; // 이미지 처리용
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_DEFAULT;
	// 이미지 처리용 Texture는 읽어올 수도 있어야(여러번 필터링 시) 
	// 하지만 메모리에 필터링 결과도 렌더링도 해야된다.
	// 현재는 Swap-Chain을 통하지 않고 별도의 Texture Memory를 만들어 렌더링
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | // Shader Resource로 사용
						D3D11_BIND_RENDER_TARGET | // 필터링 결과 렌더링
						D3D11_BIND_UNORDERED_ACCESS;
	txtDesc.MiscFlags = 0;
	txtDesc.CPUAccessFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	viewDesc.Format = txtDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	// 이미지를 저장할 Texture 생성
	ComPtr<ID3D11Texture2D> texture;
	device->CreateTexture2D(&txtDesc, NULL, texture.GetAddressOf());
	// 텍스쳐에 Shader에서 렌더링하기 위해 RenderTargetView 생성
	device->CreateRenderTargetView(texture.Get(), &viewDesc, m_renderTargetView.GetAddressOf());
	// 텍스쳐를 Shader에서 읽어오기 위해 ResourceView 생성
	device->CreateShaderResourceView(texture.Get(), nullptr, m_shaderResourceView.GetAddressOf());

	// Copute Shader를 사용하면 indexing을 더 편리하게 할 수 있다.
	// 현재 간단한 이미지이기에 Compute Shader 없이 필터링
	m_pixelConstData.dx = 1.f / width; // 픽셀의 x축 간격
	m_pixelConstData.dy = 1.f / height; // 픽셀의 y축 간격

	// Pixel Shader에서 사용할 Constant buffer 생성
	D3D11Utils::CreateConstantBuffer(device, m_pixelConstData, m_mesh->constantBufferPS);

	// 생성한 Texture의 RenderTargetView를 기본 RenderTarget으로 사용
	this->SetRenderTargets({ m_renderTargetView });
}

void ImageFilter::UpdateConstantBuffers(ComPtr<ID3D11Device>& context)
{
}
