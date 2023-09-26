#pragma once

#include "AppBase.h"
#include <vector>

#include "ConstantData.h"

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

	MeshConstData m_constantBufferData;

	// ImGUI parameters
	// 원근 투영을 사용할 것인지
	bool m_usePerspectiveProjection = true;
	bool m_wireFrame = false;
};