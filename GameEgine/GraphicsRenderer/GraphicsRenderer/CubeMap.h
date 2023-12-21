#pragma once

#include <memory>
#include <wrl.h>
#include <d3d11.h>
#include <string>

#include "Mesh.h"
#include "MeshData.h"

using Microsoft::WRL::ComPtr;

// Cubic Environmental Mapping https://learn.microsoft.com/en-us/windows/win32/direct3d9/cubic-environment-mapping
// 만약 파노라마(360) 이미지를 사용하고 싶다면 큐브가 아닌 구(Sphere) 형태를 생성해야된다.
class CubeMap {
public:
	// CubeMap 생성 (.dds 파일을 읽어서 초기화)
	void Initialize(ComPtr<ID3D11Device>& device, const std::wstring path,
		const std::wstring originalFilename, const std::wstring diffuseFilename, const std::wstring specularFileName);
	// CubeMap의 ViewProj buffer Update
	void UpdateConstantBuffer(ComPtr<ID3D11DeviceContext>& context, const Matrix& view, const Matrix& proj);
	// CubeMap Rendering (효율성을 위해 가장 마지막에 Rendering하는 것이 권장된다.)
	void Render(ComPtr<ID3D11DeviceContext>& context);

	ComPtr<ID3D11ShaderResourceView> GetTextureResView();
	ComPtr<ID3D11ShaderResourceView> GetDiffuseResView();
	ComPtr<ID3D11ShaderResourceView> GetSpecularResView();

public:
	struct CubeVSConstData {
		Matrix viewProj; // 보통 View, Projection 행렬은 미리 곱해서 사용한다.
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