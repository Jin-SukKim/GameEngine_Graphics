#include "D3D11Utils.h"

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
		0,					// shader에 Include 넣어줄 때 사용 (D3D_COMPILE_STANDARD_FILE_INCLUDE)
		"VSmain",			// Shader의 entryPoint
		"vs_5_0",			// Shader version
		0,		// Compile option
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

	HRESULT hr = D3DCompileFromFile(
		filename.c_str(), 0, 0, "PSmain", "ps_5_0", compileFlags, 0,
		&shaderBlob, &errorBlob);

	CheckResult(hr, errorBlob.Get());

	device->CreatePixelShader(
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		NULL, &pixelShader);
}

void D3D11Utils::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC bDesc = {};
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
	bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bDesc, &indexBufferData, indexBuffer.GetAddressOf());
}
