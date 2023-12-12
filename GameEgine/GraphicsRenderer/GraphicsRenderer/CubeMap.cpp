#include "CubeMap.h"
#include "D3D11Utils.h"
#include "GeometryGenerator.h"

void CubeMap::Initialize(ComPtr<ID3D11Device>& device, const std::wstring path, const std::wstring diffuseFilename, const std::wstring specularFileName)
{
	m_cubeMap = std::make_shared<Mesh>();
	// .dds ���� �о�鿩�� texture�� resourceView �ʱ�ȭ (c_str()�� wchar_t*�� ��ȯ)
	D3D11Utils::CreateCubeMapTexture(device, (path + diffuseFilename).c_str(), m_cubeMap->diffuseResView);
	D3D11Utils::CreateCubeMapTexture(device, (path + specularFileName).c_str(), m_cubeMap->specularResView);

	// Texture Sampler ����
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Sampler State ����
	device->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());

	// Constant Buffer ����
	m_cubeConstVSBufferData.viewProj = Matrix();
	D3D11Utils::CreateConstantBuffer(device, m_cubeConstVSBufferData, m_cubeVSConstantBuffer);

	// mesh ����
	MeshData mesh = GeometryGenerator::MakeCube(20.f);
	//MeshData mesh = GeometryGenerator::MakeSphere(20.f, 20, 20);

	// mesh�� index ������ �ݴ���� �޸�� �ո��� �ٲ� cubeMap ���ΰ� rendering�ǵ��� �Ѵ�.
	std::reverse(mesh.indices.begin(), mesh.indices.end());

	// mesh�� ���� ���� �� GPU�� ����
	D3D11Utils::CreateVertexBuffer(device, mesh.vertices, m_cubeMap->vertexBuffer);
	D3D11Utils::CreateVertexBuffer(device, mesh.indices, m_cubeMap->indexBuffer);
	m_cubeMap->indexCount = (UINT)mesh.indices.size();

	// Shader ����
	
	// InputLayer ������ Format ����
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout & Vertex Shader ����
	D3D11Utils::CreateVSAndInputLayout(device, L"Shader/CubeMapVS.hlsl", inputElements,
		m_cubeVS, m_cubeIL);
	// Pixel Shader ����
	D3D11Utils::CreatePS(device, L"Shader/CubeMapPS.hlsl", m_cubePS);
}

void CubeMap::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context)
{
}

void CubeMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
}
