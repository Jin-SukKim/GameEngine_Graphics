#include "CubeMap.h"

void CubeMap::Initialize(ComPtr<ID3D11Device>& device, const wchar_t* path, const wchar_t* diffuseFilename, const wchar_t* specularFileName)
{
	m_cubeMap = std::make_shared<Mesh>();
	// .dds ���� �о�鿩�� �ʱ�ȭ
	D3D11Utils::CreateCubeMapTexture(device, path.append(diffuseFilename), m_cubeMap->diffuseResView);
	D3D11Utils::CreateCubeMapTexture(device, wcscat_s(path, specularFileName), m_cubeMap->specularResView);
}

void CubeMap::UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context)
{
}

void CubeMap::Render(ComPtr<ID3D11DeviceContext>& context)
{
}
