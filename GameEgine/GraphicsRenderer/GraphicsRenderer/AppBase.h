#pragma once
#include "D3D11Utils.h"
#include "Camera.h"

// windows
#ifndef NOMINMAX
#define NOMINMAX
#endif
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

	virtual bool Initialize();
	int Run();

	void OnMouseMove(int mouseX, int mouseY);
	virtual LRESULT CALLBACK AppProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	float GetAspectRatio() const;

	// 현재 윈도우 스크린샷
	void WriteImage();

	// Pure Virtual Function이기에 자식클래스의 함수 실행
	virtual void UpdateGUI() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
protected:
	bool InitWindow();
	bool InitD3D();
	bool InitGUI();

	bool CreateRenderTargetView();
	bool CreateDepthBuffer();
	void SetViewport();

public:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_mainWindow;
	// MSAA 지원 확인
	UINT numQualityLevels;

	// Graphics Pipeline이 사용할 자원을 생성할 때는 Device interface가 사용된다.
	// Resource를 생성, Display Adapter 기능 열거
	ComPtr<ID3D11Device> m_device;
	// 생성된 자원들을 사용하거나, Graphics pipeline 자체를 조작할 때는 device context interface가 사용된다.
	// Pipeline State 설정, Resource를 사용해 렌더링 명령을 생성하는데 사용
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
	ComPtr<IDXGISwapChain> m_swapChain;
	// 그래픽을 표시하기 위해 벡터 정보를 픽셀로 변환 (Clipping, Culling, 원근 투영 등을 한다)
	ComPtr<ID3D11RasterizerState> m_SolidRasterizerState;
	ComPtr<ID3D11RasterizerState> m_WireRasterizerState;

	// Depth/Stencil 관련
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_VIEWPORT m_screenViewPort;

	Camera m_camera;

	// 눌린 키보드 입력 상태
	bool m_keyPressed[256] = { false, };
	bool m_mousePresses[2] = { false, };
};
