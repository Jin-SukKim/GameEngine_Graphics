#include "ImageFilter.h"

void ImageFilter::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const std::wstring& vertexPrefix, const std::wstring& pixelPrefix, int width, int height)
{
	// ȭ���� �������� �簢���� �ϳ� �׷��ش�.
	MeshData meshData = GeometryGenerator::MakeSquare();
	m_mesh = std::make_shared<Mesh>();

	// vertex, index buffer ����
	D3D11Utils::CreateVertexBuffer(device, meshData.vertices, m_mesh->vertexBuffer);
	D3D11Utils::CreateIndexBuffer(device, meshData.indices, m_mesh->indexBuffer);
	m_mesh->indexCount = UINT(meshData.indices.size());

	// InputLayout ����
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Shader ���� (�̹��� ���͸� ���� �پ��� Shader�� ����� �� �ֵ��� ����)
	D3D11Utils::CreateVSAndInputLayout(device, vertexPrefix + L"VS.hlsl", inputElements, m_vertexShader, m_inputLayout);
	D3D11Utils::CreatePS(device, pixelPrefix + L"PS.hlsl", m_pixelShader);

	// Texture Sampler ����
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	// Image Filter�� �Ϲ������� ����ϴ� CLAMP ���
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Texture Sampler ����
	device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

	// Rasterization ��� ���� (Image Filter�� ȿ���� �ָ� �ǹǷ� Depth buffer�� �ʿ����.)
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.DepthClipEnable = false; // depth ���� ��� X

	// ������ Rasterizer ����
	device->CreateRasterizerState(&rastDesc, m_rasterizerState.GetAddressOf());

	// ����� ���� �ػ󵵰� ���ų� ���ƾ��ϴ� ��찡 �־� viewport ����
	// Runtime���� �ٲ��൵ �ȴ�.
	ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = static_cast<float>(width);
	m_viewPort.Height = static_cast<float>(height);
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.MaxDepth = 1.0f;

	// Texture�� GPU�� �޸𸮿� ������ 2���� �̹����� �ٷ�µ� ����ȭ�Ǿ� �ִ�.
	D3D11_TEXTURE2D_DESC txtDesc;
	ZeroMemory(&txtDesc, sizeof(txtDesc));
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	// ��ǰ���� �̹��� ó���� �� �뷮�� ū 128 bits float�� ����ϴ� ��찡 ����.
	txtDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT; // �̹��� ó����
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_DEFAULT;
	// �̹��� ó���� Texture�� �о�� ���� �־��(������ ���͸� ��) 
	// ������ �޸𸮿� ���͸� ����� �������� �ؾߵȴ�.
	// ����� Swap-Chain�� ������ �ʰ� ������ Texture Memory�� ����� ������
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | // Shader Resource�� ���
						D3D11_BIND_RENDER_TARGET | // ���͸� ��� ������
						D3D11_BIND_UNORDERED_ACCESS;
	txtDesc.MiscFlags = 0;
	txtDesc.CPUAccessFlags = 0;

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
	viewDesc.Format = txtDesc.Format;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipSlice = 0;

	// �̹����� ������ Texture ����
	ComPtr<ID3D11Texture2D> texture;
	device->CreateTexture2D(&txtDesc, NULL, texture.GetAddressOf());
	// �ؽ��Ŀ� Shader���� �������ϱ� ���� RenderTargetView ����
	device->CreateRenderTargetView(texture.Get(), &viewDesc, m_renderTargetView.GetAddressOf());
	// �ؽ��ĸ� Shader���� �о���� ���� ResourceView ����
	device->CreateShaderResourceView(texture.Get(), nullptr, m_shaderResourceView.GetAddressOf());

	// Copute Shader�� ����ϸ� indexing�� �� ���ϰ� �� �� �ִ�.
	// ���� ������ �̹����̱⿡ Compute Shader ���� ���͸�
	m_pixelConstData.dx = 1.f / width; // �ȼ��� x�� ����
	m_pixelConstData.dy = 1.f / height; // �ȼ��� y�� ����

	// Pixel Shader���� ����� Constant buffer ����
	D3D11Utils::CreateConstantBuffer(device, m_pixelConstData, m_mesh->constantBufferPS);

	// ������ Texture�� RenderTargetView�� �⺻ RenderTarget���� ���
	this->SetRenderTargets({ m_renderTargetView });
}

void ImageFilter::UpdateConstantBuffers(ComPtr<ID3D11Device>& context)
{
}
