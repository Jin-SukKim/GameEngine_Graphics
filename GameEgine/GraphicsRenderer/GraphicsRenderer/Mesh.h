#pragma once
#include <wrl.h>

#include <d3d11.h>

using Microsoft::WRL::ComPtr;

// Data가 담길 Resource와 Shader가 사용해 접근할 ResourceView
struct Mesh {
	// Buffers
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	
	// Texture
	ComPtr<ID3D11Texture2D> m_meshTexture; // Texture
	// Shader Resource View : Texture를 Shader가 Resource로 사용할 수 있는 형식으로 view를 변환
	// 즉, Resource는 데이터(텍스쳐, 버퍼 등)을 의미하고 Shader는 Resource View를 통해
	// 데이터에 접근하고 사용하는 방법을 설정한다.(Read, Write 등)
	// Shader Resource는 Shader에서 resource로 사용할 것이라는 의미이고
	// View는 Texture라는 데이터를 Shader Resource라는 view로 보겠다는 의미
	// Texture를 shader의 RenderTarget으로 사용 가능하고 다시 다른 Shader에 입력도 가능하다. (입력시 ResourceView로 넣어준다)
	// 즉, 같은 Texture 메모리 공간이어도 RenderTarget이나 Resource로 사용할 수도 있으므로
	// 별도의 ResourceView를 만들어 입력으로 넣어준다.
	ComPtr<ID3D11ShaderResourceView> m_textureResourceView;
	ComPtr<ID3D11ShaderResourceView> m_diffuseResView;
	ComPtr<ID3D11ShaderResourceView> m_specularResView;

	// constant buffers
	ComPtr<ID3D11Buffer> constantBufferVS;
	ComPtr<ID3D11Buffer> constantBufferPS;

	// Rendering할 때 필요한 index의 개수
	UINT indexCount = 0;
};