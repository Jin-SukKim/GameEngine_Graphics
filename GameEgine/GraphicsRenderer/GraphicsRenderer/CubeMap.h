#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>

#include "Mesh.h"
#include "MeshData.h"
#include "ConstantData.h"

using Microsoft::WRL::ComPtr;

// Cubic Environmental Mapping https://learn.microsoft.com/en-us/windows/win32/direct3d9/cubic-environment-mapping
// 만약 파노라마(360) 이미지를 사용하고 싶다면 큐브가 아닌 구(Sphere) 형태를 생성해야된다.
class CubeMap {
public:
	// CubeMap 생성 (.dds 파일을 읽어서 초기화)
	void Initialize(ComPtr<ID3D11Device>& device, const wchar_t* path,
		const wchar_t* diffuseFilename, const wchar_t* specularFileName);
	// CubeMap의 ViewProj buffer Update
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context);
	// CubeMap Rendering (효율성을 위해 가장 마지막에 Rendering하는 것이 권장된다.)
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
	// 기본적인 CubeMap에는 필요없으나 Animation Texture 기술을 결합하면 필요하다.
	ComPtr<ID3D11Buffer> m_cubePSConstantBuffer;

	UINT m_cubeIndexCount = 0;
};