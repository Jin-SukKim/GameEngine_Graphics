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
// �ַ� shader�� buffer �����ϴ� �Լ���
class D3D11Utils {
public:

	// Inputlayout & Vertex Shader ���� (device ���)
	static void CreateVSAndInputLayout(
		ComPtr<ID3D11Device>& device, // ���۸� ������ device interface
		const std::wstring& filename,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader,
		ComPtr<ID3D11InputLayout>& inputLayout);

	// Pixel Shader ����
	static void CreatePS(
		ComPtr<ID3D11Device>& device,
		const std::wstring& filename,
		ComPtr<ID3D11PixelShader>& pixelShader);

	// Index Buffer ����
	static void CreateIndexBuffer(
		ComPtr<ID3D11Device>& device,
		const std::vector<uint32_t>& indices,
		ComPtr<ID3D11Buffer>& indexBuffer);

	// Vertex Buffer ����
	template<typename T_VERTEX>
	static void CreateVertexBuffer(
		ComPtr<ID3D11Device>& device,
		const std::vector<T_VERTEX>& vertices,
		ComPtr<ID3D11Buffer>& vertexBuffer);

	// Constant Buffer ����
	template <typename T_CONSTANT>
	static void CreateConstantBuffer(
		ComPtr<ID3D11Device>& device,
		const T_CONSTANT& constantBufferData,
		ComPtr<ID3D11Buffer>& constantBuffer);

	// Update Buffer Data  (������ Resource�� ����ϹǷ� device context ���)
	template <typename T_DATA>
	static void UpdateBuffer(
		ComPtr<ID3D11DeviceContext>& context,
		const T_DATA& bufferData,
		ComPtr<ID3D11Buffer>& buffer);

	// �̹��� ���� ������ �о����
	static void ReadImage(const char* filename, std::vector<uint8_t>& image, int& width, int& height);

	// Texture �̹��� ���� �о�鿩�� �ؽ�ó �޸� ����
	static void CreateTexture(
		ComPtr<ID3D11Device>& device, 
		const std::string& filename,
		ComPtr<ID3D11Texture2D>& texture,
		ComPtr<ID3D11ShaderResourceView>& txtResView);

	// CubeMap Texture ����
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
	bDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ���� x
	bDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size()); // �� �޸�
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0; // No CPU 
	// �� byte ������ ������ (�迭�̹Ƿ� T_VERTEX structure ũ�� ������ ������.)
	bDesc.StructureByteStride = sizeof(T_VERTEX); 

	// CPU���� GPU�� �����͸� ���� �� � �����͸� � �������� ������ ����
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // �ʱ�ȭ
	// CPU�� �������� Pointer�� �̰����� ������ ���� ����
	vertexBufferData.pSysMem = vertices.data(); // vertices�� ù ��° index ������
	// Vertex Buffer�� ���� buffer resource���� ������ Subresource�� ���� �� ����.
	// ���� vertices�� 1���� �����Ǿ� �־� 0���� ���� (2d array�̰ų� �̻��̸� �˸°� ����)��
	vertexBufferData.SysMemPitch = 0; 
	vertexBufferData.SysMemSlicePitch = 0;

	// GPU�� Buffer�� ����� �޸� �Ҵ� �޾ƿ���
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
	// �� ������ ����� �����͸� �����Ƿ�
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

	// CPU ������ -> GPU ���۷� ����
	D3D11_MAPPED_SUBRESOURCE ms;
	// Map�� �ϸ� CPU�� �޸𸮿� GPU�� �޸𸮸� ���������ش�.
	// ������ ���� ���� (������ Lock �Լ��� �ٸ� ���μ��� ���� ����)
	context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	// GPU �޸𸮿� ������ ī��
	memcpy(ms.pData, &bufferData, sizeof(bufferData));
	// ���� ���� (������ UnLock) - GPU �޸� �ݱ�
	context->Unmap(buffer.Get(), NULL);
}
