#pragma once

#include <memory>

#include <wrl.h>

#include <d3d11.h>

#include "Mesh.h"
#include "MeshData.h"
#include "ConstantData.h"

using Microsoft::WRL::ComPtr;

class MeshModel {
public:
	// 생성한 MeshData를 가지고 와서 버퍼에 초기화
	void Initialize(ComPtr<ID3D11Device>& device, MeshData& mesh);
	void Render(ComPtr<ID3D11DeviceContext>& context, bool drawNormal = false);
	void UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context);

public:
	// Constant Data
	MeshVSConstData m_constantVSBufferData;
	MeshPSConstData m_constantPSBufferData;
	MeshNormalConstData m_constantNormalBufferData;

private:
	// Mesh
	std::shared_ptr<Mesh> m_mesh;
	// Mesh Normal
	std::shared_ptr<Mesh> m_normal;

	// Shader
	ComPtr<ID3D11VertexShader> m_meshVertexShader;
	ComPtr<ID3D11PixelShader> m_meshPixelShader;
	ComPtr<ID3D11InputLayout> m_meshInputLayout;
	ComPtr<ID3D11VertexShader> m_meshNormalVertexShader;
	ComPtr<ID3D11PixelShader> m_meshNormalPixelShader;

	// Buffers
	ComPtr<ID3D11Buffer> m_meshVSConstantBuffer;
	ComPtr<ID3D11Buffer> m_meshPSConstantBuffer;
	ComPtr<ID3D11Buffer> m_meshNormalConstantBuffer;

	// 그려야되는 Index 개수
	UINT m_meshIndexCount = 0;
};