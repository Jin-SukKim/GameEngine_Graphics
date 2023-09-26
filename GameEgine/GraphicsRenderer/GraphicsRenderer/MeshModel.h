#pragma once

#include <wrl.h>

#include <d3d11.h>

#include "MeshData.h"
#include "ConstantData.h"

using Microsoft::WRL::ComPtr;

class MeshModel {
public:
	// ������ MeshData�� ������ �ͼ� ���ۿ� �ʱ�ȭ
	void Initialize(ComPtr<ID3D11Device>& device, MeshData& mesh);
	void Render(ComPtr<ID3D11DeviceContext>* context);
	void UpdateConstantBuffers(ComPtr<ID3D11DeviceContext>& context);

public:
	// Constant Data
	MeshConstData m_constantBufferData;

private:
	// Shader
	ComPtr<ID3D11VertexShader> m_meshVertexShader;
	ComPtr<ID3D11PixelShader> m_meshPixelShader;
	ComPtr<ID3D11InputLayout> m_meshInputLayout;

	// Buffers
	ComPtr<ID3D11Buffer> m_meshVSConstantBuffer;

	// �׷��ߵǴ� Index ����
	UINT m_meshIndexCount = 0;
};