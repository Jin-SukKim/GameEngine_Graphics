#include "D3D11Utils.h"

// �̹��� Lib
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// ť���
#include <directxtk/DDSTextureLoader.h>
//#include <DDSTextureLoader.h>

// https://learn.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader
void CheckResult(HRESULT hr, ID3DBlob* errorBlob)
{
	if (FAILED(hr))
	{
		// ������ ���� ���
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
			std::cout << "File not found.\n";

		// ���� �޽����� �ִ� ���
		if (errorBlob)
			std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << "\n";
	}
}

void D3D11Utils::CreateVSAndInputLayout(ComPtr<ID3D11Device>& device, const std::wstring& filename, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
{
	// �ӽ÷� ����� �����͸� ������ ����
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(
		filename.c_str(),	// Shader File Name
		0,					// macro
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// shader�� Include �־��� �� ��� (D3D_COMPILE_STANDARD_FILE_INCLUDE)
		"vsMain",			// Shader�� entryPoint
		"vs_5_0",			// Shader version
		compileFlags,		// Compile option
		0,					// Compile Option (�ַ� 0 ���)
		&shaderBlob,		// ������ ������ ����
		&errorBlob			// ������ ������ ����
	);

	CheckResult(hr, errorBlob.Get());

	// Shader�� GPU���� ����ϴ� ���α׷��� ����.

	// InputLayout�� Vertex Shader�� � �����Ͱ� ���� ����
	// vertex shader ����
	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),	// Compile�� Shader
		shaderBlob->GetBufferSize(),	// Compile�� Shader�� ũ��
		NULL,							// ClassLinkage �������̽� ������
		&vertexShader					// ������ shader �Ҵ�
	);

	// Input Layout ����
	device->CreateInputLayout(
		inputElements.data(),			// InputLayout ������ ����  �迭
		UINT(inputElements.size()),		// Input-Elements �迭 ũ��
		shaderBlob->GetBufferPointer(),	// Compile�� Shader
		shaderBlob->GetBufferSize(),	// Compile�� Shader ũ��
		&inputLayout					// ������ InputLayout �Ҵ�
	);
}

void D3D11Utils::CreatePS(ComPtr<ID3D11Device>& device, const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// shader���� include ���
	HRESULT hr = D3DCompileFromFile(
		filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "psMain", 
		"ps_5_0", compileFlags, 0, &shaderBlob, &errorBlob);

	CheckResult(hr, errorBlob.Get());

	device->CreatePixelShader(
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		NULL, &pixelShader);
}

void D3D11Utils::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.StructureByteStride = sizeof(uint32_t);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bDesc, &indexBufferData, indexBuffer.GetAddressOf());
}

void D3D11Utils::ReadImage(const char* filename, std::vector<uint8_t>& image, int& width, int& height)
{
	// �̹����� channel ����
	int channels;

	// Image�κ��� ������ �о����
	// 1����ó�� �̹������� �����͸� �����´�. (���� ������ [0, 255]�̹Ƿ� 8bit�� ������ �ȴ�)
	unsigned char* img =
		stbi_load(filename, &width, &height, &channels, 0);

	// ���� �̹��� ������ ����
	image.resize(width * height * 4); // 4ä�η� ���� ���� (RGBA)
	// �´� ä�ο� ���� ����

	switch (channels)
	{
	case 1:
		for (size_t i = 0; i < width * height; i++)
		{
			uint8_t g = img[i * channels + 0];
			for (size_t c = 0; c < 4; c++)
				image[4 * i + c] = g;
		}
		break;
	case 2:
		for (size_t i = 0; i < width * height; i++)
		{
			for (size_t c = 0; c < 2; c++)
				image[4 * i + c] = img[i * channels + c];
			image[4 * i + 2] = img[i * channels + 2];
			image[4 * i + 3] = img[i * channels + 3];

		}
		break;
	case 3:
		for (size_t i = 0; i < width * height; i++)
		{
			for (size_t c = 0; c < 3; c++)
				image[4 * i + c] = img[i * channels + c];
			image[4 * i + 3] = img[i * channels + 3];
		}
		break;
	case 4:
		for (size_t i = 0; i < width * height; i++)
		{
			for (size_t c = 0; c < 4; c++)
				image[4 * i + c] = img[i * channels + c];
		}
		break;
	default:
		std::cout << "Cannot read " << channels << " channels\n";
		break;
	}
	delete[] img;
}

void D3D11Utils::CreateTexture(ComPtr<ID3D11Device>& device, const std::string& filename,
	ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
	if (filename.empty())
		return;
	// �̹����� Data
	int width = 0, height = 0;
	// �Ϲ������� ����ϴ� �̹��� Ÿ�� (RGBA�� �� 256�̹Ƿ� 8bit�� �ȴ�)
	std::vector<uint8_t> image;

	ReadImage(filename.c_str(), image, width, height);

	// Texture ����
	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	// �̹��� ������ �о���� Data�� ������ Texture ���� �� ����� Format
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	// �� �� ���� Texture Image�� �ٽ� ������ ���� ���⿡ immutable�� ����
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	// Shader Resource�� ��� (RenderTarget���� ���� RenderTarget���� ����)
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Image�� �����ͷ� �ʱ�ȭ - Subresource Data�� ����
	// Subresource�� resource���� �ִ� ���� �����͸� �ǹ� (Resource�� Subresource�� ����)
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image.data();
	initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4; // 4 ä�� ���

	// �о�� �̹��� �����͸� ����� Texture ����
	device->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
	// ������ Texture�� TextureResourceView ����
	device->CreateShaderResourceView(texture.Get(), nullptr,
		textureResourceView.GetAddressOf());
}

void D3D11Utils::CreateCubeMapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView)
{
	// .dds ���� �о�鿩�� �ʱ�ȭ (ť�� ����)
	ComPtr<ID3D11Texture2D> texture;

	// https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
	// CreateDDSTextureFromFileEx() �Լ��� ����� Texture��
	// Texture�� ResourceView�� �ʱ�ȭ�� �� �ִ�.
	auto hr = DirectX::CreateDDSTextureFromFileEx(
		device.Get(), 
		filename, 
		0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // ť��ʿ� �ؽ���
		DirectX::DDS_LOADER_FLAGS(false),
		// �о���� �����͸� �ӽ÷� ������ Texture
		// Shader�� �ѱ� �� ResourceView�� ������ �Ǳ⿡ Texture�� �ӽ÷� ����Ѵ�.
		(ID3D11Resource**)texture.GetAddressOf(),
		texResView.GetAddressOf(), 
		nullptr);

	if (FAILED(hr)) {
		std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
	}
}