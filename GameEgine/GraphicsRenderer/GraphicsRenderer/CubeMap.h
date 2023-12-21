#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include <string>

#include "Mesh.h"
#include "MeshData.h"

using Microsoft::WRL::ComPtr;

// Cubic Environmental Mapping https://learn.microsoft.com/en-us/windows/win32/direct3d9/cubic-environment-mapping
// ���� �ĳ��(360) �̹����� ����ϰ� �ʹٸ� ť�갡 �ƴ� ��(Sphere) ���¸� �����ؾߵȴ�.
class CubeMap {
public:
	// CubeMap ���� (.dds ������ �о �ʱ�ȭ)
	void Initialize(ComPtr<ID3D11Device>& device, const std::wstring path,
		const std::wstring originalFilename, const std::wstring diffuseFilename, const std::wstring specularFileName);
	// CubeMap�� ViewProj buffer Update
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context, const Matrix& view, const Matrix& proj);
	// CubeMap Rendering (ȿ������ ���� ���� �������� Rendering�ϴ� ���� ����ȴ�.)
	void Render(ComPtr<ID3D11DeviceContext>& context);

	ComPtr<ID3D11ShaderResourceView> GetTextureResView();
	ComPtr<ID3D11ShaderResourceView> GetDiffuseResView();
	ComPtr<ID3D11ShaderResourceView> GetSpecularResView();

public:
	struct CubeVSConstData {
		Matrix viewProj; // ���� View, Projection ����� �̸� ���ؼ� ����Ѵ�.
	};
	CubeVSConstData m_cubeConstVSBufferData;
private:
	// Mesh Data
	std::shared_ptr<Mesh> m_cubeMap;

	// Shader
	ComPtr<ID3D11InputLayout> m_cubeIL;
	ComPtr<ID3D11VertexShader> m_cubeVS;
	ComPtr<ID3D11PixelShader> m_cubePS;
	
	// Texture Sampler
	ComPtr<ID3D11SamplerState> m_samplerState;

	UINT m_cubeIndexCount = 0;
};