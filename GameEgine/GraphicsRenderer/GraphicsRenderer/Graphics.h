#pragma once

#include "AppBase.h"
#include <vector>
// DirectX�� Math Library�� �� �� �����ϰ� ����� �� �ִ� Library
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

struct Vertex {
	Vector3 position;
	Vector3 color;
};

// 16 byte ���� (float 4��) ������ ����
// Matrix = XMFLOAT4 = float 4��
struct MeshConstant {
	Matrix world;
	Matrix view;
	Matrix proj;
};

class Graphics : public AppBase {
public:
	Graphics();

	virtual bool Initialize() override;
	virtual void Update(float dt) override;
	virtual void Render() override;
	virtual void UpdateGUI() override;

protected:
	// Shaders
	ComPtr<ID3D11VertexShader> m_vertexShader;
	ComPtr<ID3D11PixelShader> m_pixelShader;
	ComPtr<ID3D11InputLayout> m_inputLayout;

	// Buffers
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	ComPtr<ID3D11Buffer> m_constantBuffer;
	UINT m_indexCount;

	MeshConstant m_constantBufferData;

	// ���� ������ ����� ������
	bool m_usePerspectiveProjection = true;
};