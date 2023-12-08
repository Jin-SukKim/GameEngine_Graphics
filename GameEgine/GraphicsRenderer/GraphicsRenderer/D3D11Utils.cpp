#include "D3D11Utils.h"

// 이미지 Lib
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// 큐브맵
#include <directxtk/DDSTextureLoader.h>
//#include <DDSTextureLoader.h>

// https://learn.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader
void CheckResult(HRESULT hr, ID3DBlob* errorBlob)
{
	if (FAILED(hr))
	{
		// 파일이 없는 경우
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0)
			std::cout << "File not found.\n";

		// 에러 메시지가 있는 경우
		if (errorBlob)
			std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << "\n";
	}
}

void D3D11Utils::CreateVSAndInputLayout(ComPtr<ID3D11Device>& device, const std::wstring& filename, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
{
	// 임시로 사용할 데이터를 저장할 공간
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(
		filename.c_str(),	// Shader File Name
		0,					// macro
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	// shader에 Include 넣어줄 때 사용 (D3D_COMPILE_STANDARD_FILE_INCLUDE)
		"vsMain",			// Shader의 entryPoint
		"vs_5_0",			// Shader version
		compileFlags,		// Compile option
		0,					// Compile Option (주로 0 사용)
		&shaderBlob,		// 임의의 데이터 공간
		&errorBlob			// 임의의 데이터 공간
	);

	CheckResult(hr, errorBlob.Get());

	// Shader는 GPU에서 사용하는 프로그램과 같다.

	// InputLayout이 Vertex Shader로 어떤 데이터가 들어갈지 지정
	// vertex shader 생성
	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),	// Compile한 Shader
		shaderBlob->GetBufferSize(),	// Compile한 Shader의 크기
		NULL,							// ClassLinkage 인터페이스 포인터
		&vertexShader					// 생성한 shader 할당
	);

	// Input Layout 생성
	device->CreateInputLayout(
		inputElements.data(),			// InputLayout 데이터 형식  배열
		UINT(inputElements.size()),		// Input-Elements 배열 크기
		shaderBlob->GetBufferPointer(),	// Compile한 Shader
		shaderBlob->GetBufferSize(),	// Compile한 Shader 크기
		&inputLayout					// 생성한 InputLayout 할당
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

	// shader에서 include 사용
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
	// 이미지의 channel 개수
	int channels;

	// Image로부터 데이터 읽어오기
	// 1차원처럼 이미지에서 데이터를 가져온다. (색의 범위는 [0, 255]이므로 8bit만 있으면 된다)
	unsigned char* img =
		stbi_load(filename, &width, &height, &channels, 0);

	// 읽은 이미지 데이터 복사
	image.resize(width * height * 4); // 4채널로 만들어서 복사 (RGBA)
	// 맞는 채널에 따라 복사

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
	// 이미지의 Data
	int width = 0, height = 0;
	// 일반적으로 사용하는 이미지 타입 (RGBA값 당 256이므로 8bit면 된다)
	std::vector<uint8_t> image;

	ReadImage(filename.c_str(), image, width, height);

	// Texture 생성
	D3D11_TEXTURE2D_DESC txtDesc = {};
	txtDesc.Width = width;
	txtDesc.Height = height;
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	// 이미지 파일을 읽어들인 Data를 가지고 Texture 생성 시 사용할 Format
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	txtDesc.SampleDesc.Count = 1;
	// 한 번 읽은 Texture Image를 다시 변형할 일은 없기에 immutable로 설정
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	// Shader Resource로 사용 (RenderTarget으로 사용시 RenderTarget으로 설정)
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Image의 데이터로 초기화 - Subresource Data를 설정
	// Subresource는 resource내에 있는 실제 데이터를 의미 (Resource는 Subresource의 집합)
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = image.data();
	initData.SysMemPitch = txtDesc.Width * sizeof(uint8_t) * 4; // 4 채널 사용

	// 읽어온 이미지 데이터를 사용해 Texture 생성
	device->CreateTexture2D(&txtDesc, &initData, texture.GetAddressOf());
	// 생성한 Texture로 TextureResourceView 생성
	device->CreateShaderResourceView(texture.Get(), nullptr,
		textureResourceView.GetAddressOf());
}

void D3D11Utils::CreateCubeMapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView)
{
	// .dds 파일 읽어들여서 초기화 (큐브 매핑)
	ComPtr<ID3D11Texture2D> texture;

	// https://github.com/microsoft/DirectXTK/wiki/DDSTextureLoader
	// CreateDDSTextureFromFileEx() 함수를 사용해 Texture와
	// Texture의 ResourceView를 초기화할 수 있다.
	auto hr = DirectX::CreateDDSTextureFromFileEx(
		device.Get(), 
		filename, 
		0, D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, // 큐브맵용 텍스춰
		DirectX::DDS_LOADER_FLAGS(false),
		// 읽어들인 데이터를 임시로 저장할 Texture
		// Shader로 넘길 때 ResourceView만 있으면 되기에 Texture는 임시로 사용한다.
		(ID3D11Resource**)texture.GetAddressOf(),
		texResView.GetAddressOf(), 
		nullptr);

	if (FAILED(hr)) {
		std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
	}
}