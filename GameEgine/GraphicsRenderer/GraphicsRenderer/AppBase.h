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
/* ����: ������ ���� ��� ���� �߻� ���

   // MS ����
   inline void ThrowIfFailed(HRESULT hr)
   {
	   if (FAILED(hr))
	   {
		   // Set a breakpoint on this line to catch Win32 API errors.
		   throw Platform::Exception::CreateException(hr);
	   }
   }

   // �Լ� ��� ��ũ�θ� ����� ����ϴ� ���
   // Luna DX12 ����
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

	// Pure Virtual Function�̱⿡ �ڽ�Ŭ������ �Լ� ����
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

	// Graphics Pipeline�� ����� �ڿ��� ������ ���� Device interface�� ���ȴ�.
	// Resource�� ����, Display Adapter ��� ����
	ComPtr<ID3D11Device> m_device;
	// ������ �ڿ����� ����ϰų�, Graphics pipeline ��ü�� ������ ���� device context interface�� ���ȴ�.
	// Pipeline State ����, Resource�� ����� ������ ����� �����ϴµ� ���
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	// Depth/Stencil ����
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_VIEWPORT m_screenViewPort;
};
