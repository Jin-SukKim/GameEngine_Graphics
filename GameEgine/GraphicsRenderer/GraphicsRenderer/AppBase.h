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

class AppBase {
public:
	AppBase();
	~AppBase();

	LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool Initialize();
	int Run();
protected:
	bool InitWindow();
	bool InitD3D();
	bool InitGUI();
	void UpdateGUI();

public:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_mainWindow;

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_context;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	// Depth/Stencil ฐüทร
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	D3D11_VIEWPORT m_screenViewPort;

};