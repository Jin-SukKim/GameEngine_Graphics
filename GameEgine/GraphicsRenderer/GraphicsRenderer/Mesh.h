#pragma once
#include <wrl.h>

#include <d3d11.h>

using Microsoft::WRL::ComPtr;
struct Mesh {
	// Buffers
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	
	// Texture
	ComPtr<ID3D11Texture2D> m_meshTexture; // Texture
	// Shader Resource View : Texture�� Shader�� Resource�� ����� �� �ִ� �������� view�� ��ȯ
	// Shader Resource�� Shader���� resource�� ����� ���̶�� �ǹ��̰�
	// View�� Texture��� �����͸� Shader Resource��� view�� ���ڴٴ� �ǹ�
	// Texture�� shader�� RenderTarget���� ��� �����ϰ� �ٽ� �ٸ� Shader�� �Էµ� �����ϴ�. (�Է½� ResourceView�� �־��ش�)
	// ��, ���� Texture �޸� �����̾ RenderTarget�̳� Resource�� ����� ���� �����Ƿ�
	// ������ ResourceView�� ����� �Է����� �־��ش�.
	ComPtr<ID3D11ShaderResourceView> m_textureResourceView;

	// constant buffers
	ComPtr<ID3D11Buffer> constantBufferVS;
	ComPtr<ID3D11Buffer> constantBufferPS;

	// Rendering�� �� �ʿ��� index�� ����
	UINT indexCount = 0;
};