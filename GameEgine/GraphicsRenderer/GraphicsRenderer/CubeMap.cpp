#include "CubeMap.h"
#include "D3D11Utils.h"
#include "GeometryGenerator.h"

void CubeMap::Initialize(ComPtr<ID3D11Device>& device, const std::wstring path, const std::wstring originalFilename, const std::wstring diffuseFilename, const std::wstring specularFileName)
{
	m_cubeMap = std::make_shared<Mesh>();
	// mesh 생성
	//MeshData mesh = GeometryGenerator::MakeCube(20.f);
	MeshData mesh = GeometryGenerator::MakeSphere(20.f, 100, 100);

	// mesh의 index 순서를 반대로해 뒷면과 앞면을 바꿔 cubeMap 내부가 rendering되도록 한다.
	std::reverse(mesh.indices.begin(), mesh.indices.end());

	// .dds 파일 읽어들여서 texture의 resourceView 초기화 (c_str()로 wchar_t*로 변환)
	// Pre-filter가 되지 않은 원본 Texture
	D3D11Utils::CreateCubeMapTexture(device, (path + originalFilename).c_str(), m_cubeMap->txtResView);
	// // IBL(Image-Based Lighting)을 위해 다른 물체들 그릴때 사용    
	D3D11Utils::CreateCubeMapTexture(device, (path + diffuseFilename).c_str(), m_cubeMap->diffuseResView);
	D3D11Utils::CreateCubeMapTexture(device, (path + specularFileName).c_str(), m_cubeMap->specularResView);

	// Texture Sampler 설정
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Sampler State 설정
	device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

	// Constant Buffer 생성
	m_cubeConstVSBufferData.viewProj = Matrix();
	D3D11Utils::CreateConstantBuffer(device, m_cubeConstVSBufferData, m_cubeMap->constantBufferVS);

	// mesh의 버퍼 생성 후 GPU로 복사
	D3D11Utils::CreateVertexBuffer(device, mesh.vertices, m_cubeMap->vertexBuffer);
	D3D11Utils::CreateIndexBuffer(device, mesh.indices, m_cubeMap->indexBuffer);
	m_cubeMap->indexCount = (UINT)mesh.indices.size();

	// Shader 생성
	
	// InputLayer 데이터 Format 설정
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout & Vertex Shader 생성
	D3D11Utils::CreateVSAndInputLayout(device, L"Shader/CubeMapVS.hlsl", inputElements,
		m_cubeVS, m_cubeIL);
	// Pixel Shader 생성
	D3D11Utils::CreatePS(device, L"Shader/CubeMapPS.hlsl", m_cubePS);
}

void CubeMap::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context, const Matrix& view, const Matrix& proj)
{
	// Transpose 해서 들어온 값
	m_cubeConstVSBufferData.viewProj = proj * view;
	// 기본적인 CubeMap에는 Pixel Shader의 ConstantBuffer는 필요없으나 Animation Texture 기술을 결합하면 필요하다.
	D3D11Utils::UpdateBuffer(context, m_cubeConstVSBufferData, m_cubeMap->constantBufferVS);
}

void CubeMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
	// Vertex Buffer의 단위와 offset 설정
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetInputLayout(m_cubeIL.Get());
	context->IASetVertexBuffers(0, 1, m_cubeMap->vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_cubeMap->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	context->VSSetShader(m_cubeVS.Get(), 0, 0);
	context->VSSetConstantBuffers(0, 1, m_cubeMap->constantBufferVS.GetAddressOf());
	
	ID3D11ShaderResourceView* pixelResources[1] = {
		m_cubeMap->txtResView.Get()
	};

	context->PSSetShader(m_cubePS.Get(), 0, 0);
	context->PSSetShaderResources(0, 1, pixelResources);
	context->PSSetSamplers(0, 1, m_samplerState.GetAddressOf());

	context->DrawIndexed(m_cubeMap->indexCount, 0, 0);
}
