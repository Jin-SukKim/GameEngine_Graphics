#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>

#include "Mesh.h"
#include "MeshData.h"
#include "ConstantData.h"

using Microsoft::WRL::ComPtr;

// Cubic Environmental Mapping https://learn.microsoft.com/en-us/windows/win32/direct3d9/cubic-environment-mapping
// ���� �ĳ��(360) �̹����� ����ϰ� �ʹٸ� ť�갡 �ƴ� ��(Sphere) ���¸� �����ؾߵȴ�.
class CubeMap {
public:
	// CubeMap ���� (.dds ������ �о �ʱ�ȭ)
	void Initialize(ComPtr<ID3D11Device>& device, const wchar_t* path,
		const wchar_t* diffuseFilename, const wchar_t* specularFileName);
	// CubeMap�� ViewProj buffer Update
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context);
	// CubeMap Rendering (ȿ������ ���� ���� �������� Rendering�ϴ� ���� ����ȴ�.)
	void Render(ComPtr<ID3D11DeviceContext>& context);

public:
	CubeVSConstData m_cubeConstVSbuffer;
private:
	// Mesh Data
	std::shared_ptr<Mesh> cube;

	// Shader
	ComPtr<ID3D11InputLayout> m_cubeIL;
	ComPtr<ID3D11VertexShader> m_cubeVS;
	ComPtr<ID3D11PixelShader> m_cubePS;
	
	// Texture Sampler
	ComPtr<ID3D11SamplerState> m_samplerState;

	// Buffers
	ComPtr<ID3D11Buffer> m_cubeVSConstantBuffer;
	// �⺻���� CubeMap���� �ʿ������ Animation Texture ����� �����ϸ� �ʿ��ϴ�.
	ComPtr<ID3D11Buffer> m_cubePSConstantBuffer;

	UINT m_cubeIndexCount = 0;
};