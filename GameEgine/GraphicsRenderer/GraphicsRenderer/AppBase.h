#include <iostream>
#include <vector>
#include <string>
// windows
#include <windows.h>
#include <wrl.h> // ComPtr

// ImGUI
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;
/* 참고: 오류가 있을 경우 예외 발생 방법

   // MS 예제
   inline void ThrowIfFailed(HRESULT hr)
   {
	   if (FAILED(hr))
	   {
		   // Set a breakpoint on this line to catch Win32 API errors.
		   throw Platform::Exception::CreateException(hr);
	   }
   }

   // 함수 대신 매크로를 만들어 사용하는 방법
   // Luna DX12 교재
   #ifndef ThrowIfFailed
   #define ThrowIfFailed(x)                                              \
   {                                                                     \
	   HRESULT hr__ = (x);                                               \
	   std::wstring wfn = AnsiToWString(__FILE__);                       \
	   if(FAILED(hr__)) { throw DxException(hr__, L#x, wfn, __LINE__); } \
   }
   #endif
   */
class AppBase {
public:
	AppBase();
	~AppBase();

	virtual LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Initialize();
	int Run();

	float GetAspectRatio() const;

	// Pure Virtual Function이기에 자식클래스의 함수 실행
	virtual void UpdateGUI() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
protected:
	bool InitWindow();
	bool InitD3D();
	bool InitGUI();

	// Inputlayout & Vertex Shader 생성
	void CreateVSAndInputLayout(
		const std::wstring& filename,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& vertexShader,
		ComPtr<ID3D11InputLayout>& inputLayout);

	// Pixel Shader 생성
	void CreatePS(
		const std::wstring& filename,
		ComPtr<ID3D11PixelShader>& pixelShader);

	// Index Buffer 생성
	void CreateIndexBuffer(
		const std::vector<uint16_t>& indices,
		ComPtr<ID3D11Buffer>& indexBuffer);
	
	// Vertex Buffer 생성
	template<typename T_VERTEX>
	void CreateVertexBuffer(
		const std::vector<T_VERTEX>& vertices,
		ComPtr<ID3D11Buffer>& vertexBuffer);

	// Constant Buffer 생성
	template <typename T_CONSTANT>
	void CreateConstantBuffer(
		const T_CONSTANT& constantBufferData,
		ComPtr<ID3D11Buffer>& constantBuffer);

	// Update Buffer Data 
	template <typename T_DATA>
	void UpdateBuffer(
		const T_DATA& bufferData,
		ComPtr<ID3D11Buffer>& buffer);

public:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_mainWindow;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	// Depth/Stencil 관련
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_VIEWPORT m_screenViewPort;

};

template<typename T_VERTEX>
inline void AppBase::CreateVertexBuffer(const std::vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
{
	D3D11_BUFFER_DESC bDesc;
	ZeroMemory(&bDesc, sizeof(bDesc));
	bDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경 x
	bDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size()); // 총 메모리
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = 0; // No CPU Acess
	bDesc.StructureByteStride = sizeof(T_VERTEX); // 몇 비트 단위로 나눌지

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // 초기화
	// CPU의 데이터의 Pointer로 이곳부터 데이터 전송 지정
	vertexBufferData.pSysMem = vertices.data();
	// Vertex Buffer와 같은 buffer resource에선 복수의 Subresource를 가질 수 없다.
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	// GPU에 Buffer로 사용할 메모리 할당 받아오기
	const HRESULT hr = m_device->CreateBuffer(
		&bDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateVertexBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_CONSTANT>
inline void AppBase::CreateConstantBuffer(const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
{
	D3D11_BUFFER_DESC cdDesc;
	cdDesc.ByteWidth = sizeof(constantBufferData);
	// 매 프레임 변경될 데이터를 가지므로
	cdDesc.Usage = D3D11_USAGE_DYNAMIC; 
	cdDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cdDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	cdDesc.MiscFlags = 0;
	cdDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cdData;
	cdData.pSysMem = &constantBufferData;	// constant data
	cdData.SysMemPitch = 0;
	cdData.SysMemSlicePitch = 0;

	auto hr = m_device->CreateBuffer(&cdDesc, &cdData, constantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateConstantBuffer() failed. " << std::hex << hr << "\n";
	}
}

template<typename T_DATA>
inline void AppBase::UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
{
	if (!buffer)
	{
		std::cout << "UpdateBuffer() buffer was not initialized.\n";
	}

	// CPU 데이터 -> GPU 버퍼로 전송
	D3D11_MAPPED_SUBRESOURCE ms;
	// 매핑할 버퍼 설정 (일종의 Lock 함수로 다른 프로세스 접근 제한)
	m_context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	// GPU 메모리에 데이터 카피
	memcpy(ms.pData, &bufferData, sizeof(bufferData));
	// 매핑 해제 (일종의 UnLock)
	m_context->Unmap(buffer.Get(), NULL);
}
