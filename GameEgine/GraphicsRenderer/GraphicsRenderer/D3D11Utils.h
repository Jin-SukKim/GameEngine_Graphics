#pragma once
#include <vector>
#include <string>
#include <iostream>

#include <wrl.h> // ComPtr

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

// DirectX 3D 11 Utilities 
// 주로 shader와 buffer 생성하는 함수들
class D3D11Utils {
public:

	// Inputlayout & Vertex Shader 생성 (device 사용)
	static void CreateVSAndInputLayout(
		ComPtr<ID3D11Device>& device, // 버퍼를 생성할 device interface
		const std::wstring& filename,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader,
		ComPtr<ID3D11InputLayout>& inputLayout);

	// Pixel Shader 생성
	static void CreatePS(
		ComPtr<ID3D11Device>& device,
		const std::wstring& filename,
		ComPtr<ID3D11PixelShader>& pixelShader);

	// Index Buffer 생성
	static void CreateIndexBuffer(
		ComPtr<ID3D11Device>& device,
		const std::vector<uint32_t>& indices,
		ComPtr<ID3D11Buffer>& indexBuffer);

	// Vertex Buffer 생성
	template<typename T_VERTEX>
	static void CreateVertexBuffer(
		ComPtr<ID3D11Device>& device,
		const std::vector<T_VERTEX>& vertices,
		ComPtr<ID3D11Buffer>& vertexBuffer);

	// Constant Buffer 생성
	template <typename T_CONSTANT>
	static void CreateConstantBuffer(
		ComPtr<ID3D11Device>& device,
		const T_CONSTANT& constantBufferData,
		ComPtr<ID3D11Buffer>& constantBuffer);

	// Update Buffer Data  (생성한 Resource를 사용하므로 device context 사용)
	template <typename T_DATA>
	static void UpdateBuffer(
		ComPtr<ID3D11DeviceContext>& context,
		const T_DATA& bufferData,
		ComPtr<ID3D11Buffer>& buffer);

	// 이미지 파일 데이터 읽어오기
	static void ReadImage(const char* filename, std::vector<uint8_t>& image, int& width, int& height);

	// Texture 이미지 파일 읽어들여서 텍스처 메모리 생성
	static void CreateTexture(
		ComPtr<ID3D11Device>& device, 
		const std::string& filename,
		ComPtr<ID3D11Texture2D>& texture,
		ComPtr<ID3D11ShaderResourceView>& txtResView);

	// CubeMap Texture 생성
	static void CreateCubeMapTexture(
		ComPtr<ID3D11Device>& device,
		const wchar_t* filename,
		ComPtr<ID3D11ShaderResourceView>& texResView);
};


template<typename T_VERTEX>
inline void D3D11Utils::CreateVertexBuffer(ComPtr<ID3D11Device>& device, const std::vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
{
	D3D11_BUFFER_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경 x
	bDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size()); // 총 메모리
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0; // No CPU 
	// 몇 byte 단위로 나눌지 (배열이므로 T_VERTEX structure 크기 단위로 나눈다.)
	bDesc.StructureByteStride = sizeof(T_VERTEX); 

	// CPU에서 GPU로 데이터를 보낼 때 어떤 데이터를 어떤 형식으로 보낼지 설정
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // 초기화
	// CPU의 데이터의 Pointer로 이곳부터 데이터 전송 지정
	vertexBufferData.pSysMem = vertices.data(); // vertices의 첫 번째 index 포인터
	// Vertex Buffer와 같은 buffer resource에선 복수의 Subresource를 가질 수 없다.
	// 현재 vertices는 1열로 나열되어 있어 0으로 설정 (2d array이거나 이상이면 알맞게 설정)ㄴ
	vertexBufferData.SysMemPitch = 0; 
	vertexBufferData.SysMemSlicePitch = 0;

	// GPU에 Buffer로 사용할 메모리 할당 받아오기
	const HRESULT hr = device->CreateBuffer(
		&bDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateVertexBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_CONSTANT>
inline void D3D11Utils::CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(constantBufferData);
	// 매 프레임 변경될 데이터를 가지므로
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cbData;
	cbData.pSysMem = &constantBufferData;	// constant data
	cbData.SysMemPitch = 0;
	cbData.SysMemSlicePitch = 0;

	auto hr = device->CreateBuffer(&cbDesc, &cbData,
		constantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateConstantBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_DATA>
inline void D3D11Utils::UpdateBuffer(ComPtr<ID3D11DeviceContext>& context, const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
{
	if (!buffer)
	{
		std::cout << "UpdateBuffer() buffer was not initialized.\n";
	}

	// CPU 데이터 -> GPU 버퍼로 전송
	D3D11_MAPPED_SUBRESOURCE ms;
	// Map을 하면 CPU의 메모리와 GPU의 메모리를 대응시켜준다.
	// 매핑할 버퍼 설정 (일종의 Lock 함수로 다른 프로세스 접근 제한)
	context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	// GPU 메모리에 데이터 카피
	memcpy(ms.pData, &bufferData, sizeof(bufferData));
	// 매핑 해제 (일종의 UnLock) - GPU 메모리 닫기
	context->Unmap(buffer.Get(), NULL);
}
