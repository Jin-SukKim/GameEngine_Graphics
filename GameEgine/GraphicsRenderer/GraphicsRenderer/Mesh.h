#pragma once
#include <wrl.h>

#include <d3d11.h>

using Microsoft::WRL::ComPtr;
struct Mesh {
	// Buffers
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	// constant buffers
	ComPtr<ID3D11Buffer> constantBuffer;

	// Rendering할 때 필요한 index의 개수
	UINT indexCount = 0;
};