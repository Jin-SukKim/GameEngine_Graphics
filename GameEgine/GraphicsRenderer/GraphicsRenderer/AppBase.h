#include "D3D11Utils.h"

// windows
#include <windows.h>
// #include <wrl.h> // ComPtr

// ImGUI
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// DirectX
//#include <d3d11.h>
//#include <d3dcompiler.h>

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

public:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_mainWindow;

	// Graphics Pipeline이 사용할 자원을 생성할 때는 Device interface가 사용된다.
	// Resource를 생성, Display Adapter 기능 열거
	ComPtr<ID3D11Device> m_device;
	// 생성된 자원들을 사용하거나, Graphics pipeline 자체를 조작할 때는 device context interface가 사용된다.
	// Pipeline State 설정, Resource를 사용해 렌더링 명령을 생성하는데 사용
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
