#pragma once

#include <memory>

#include <wrl.h>

#include <d3d11.h>

#include "Mesh.h"
#include "MeshData.h"
#include "ConstantData.h"
#include "CubeMap.h"

using Microsoft::WRL::ComPtr;

class MeshModel {
public:
	// ������ MeshData�� ������ �ͼ� ���ۿ� �ʱ�ȭ
	void Initialize(ComPtr<ID3D11Device>& device, const std::vector<MeshData>& meshes);
	// Model File�κ��� �����͸� �о�ͼ� �ʱ�ȭ
	void Initialize(ComPtr<ID3D11Device>& device, const std::string& basePath, const std::string& fileName);
	void Render(ComPtr<ID3D11DeviceContext>& context, CubeMap& cubeMap);
	void UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context);

public:
	// Constant Data
	MeshVSConstData m_constantVSBufferData;
	MeshPSConstData m_constantPSBufferData;
	MeshNormalConstData m_constantNormalBufferData;

	bool useTexture = false;
	bool drawNormal = false;
	float normalScale = 0.005f;
private:
	// Mesh
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	// Mesh Normal
	std::shared_ptr<Mesh> m_normal;

	// Shader
	ComPtr<ID3D11VertexShader> m_meshVertexShader;
	ComPtr<ID3D11PixelShader> m_meshPixelShader;
	ComPtr<ID3D11InputLayout> m_meshInputLayout;
	ComPtr<ID3D11VertexShader> m_meshNormalVertexShader;
	ComPtr<ID3D11PixelShader> m_meshNormalPixelShader;

	// Texture�� Sampling�� �� ����ϴ� Sampler
	ComPtr<ID3D11SamplerState> m_samplerState;

	// Buffers
	ComPtr<ID3D11Buffer> m_meshVSConstantBuffer;
	ComPtr<ID3D11Buffer> m_meshPSConstantBuffer;
	ComPtr<ID3D11Buffer> m_meshNormalConstantBuffer;
};