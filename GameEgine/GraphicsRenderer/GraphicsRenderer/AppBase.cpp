#include "AppBase.h"
#include <iostream>
#include <algorithm>

#include <omp.h> // ����

// Ŭ���� ��� �Լ����� ���������� ������ �޽�����
// ó���� �� �ֵ��� ����
AppBase* app = nullptr;

// imgui_impl_wind32�� ���ǵ� �޽��� ó�� �Լ�
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

// DispatchMessage �Լ��� ȣ�� 
// (hwnd = window �ڵ�, uMsg = �޽��� �ڵ�, wParam, lParam = �޽����� ���õ� �߰� ������)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return app->AppProc(hwnd, uMsg, wParam, lParam);
}

AppBase::AppBase() : m_screenWidth(1920), m_screenHeight(1080), m_mainWindow(0),
m_screenViewPort(D3D11_VIEWPORT()) { // viewPort ���� 

	app = this;

	m_camera = Camera({ 0.0f, 0.0f, -0.5f }, { 0.0f, 0.0f, 0.0f });
	m_camera.SetAspectRatio(GetAspectRatio());
} 

AppBase::~AppBase()
{
	app = nullptr;

	// ImGUI Clear
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	
	// Window Clear
	DestroyWindow(m_mainWindow);
}

bool AppBase::Initialize()
{
	if (!InitWindow())
		return false;
	if (!InitD3D())
		return false;
	if (!InitGUI())
		return false;
	return true;
}

int AppBase::Run()
{
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// GUI ���
			// GUI ������ ����
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// �������� �� ��Ͻ���
			ImGui::Begin("Scene Control");

			// ImGui�� �������ִ� Framerate ���
			ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate);

			// GUI ���� ������Ʈ
			UpdateGUI();

			ImGui::End(); // �������
			ImGui::Render(); 
			
			// Game Update - Rendering�� �����͸� ������Ʈ
			Update(ImGui::GetIO().DeltaTime);

			// DirectX Rendering - ������Ʈ�� �����ͷ� �޸𸮿� Rendering
			Render();

			// GUI Rendering
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			// GUI ������ �� Present() ȣ��
			// �׸� Memory�� Back-buffer�� Front-Buffer�� swap�� ȭ�鿡 ���
			m_swapChain->Present(1, 0); // swap-chain�� �� ���� swap
		}
	}
	return 0;
}

void AppBase::OnMouseMove(int mouseX, int mouseY)
{
	// �������� ���콺 ��ġ ������ ���� ����� (0, 0), ���� �ϴ��� (width - 1, height -1)�̴�.
	// �̰� NDC ��ǥ ������ ��ȯ���� ����Ѵ�. 
	// NDC ��ǥ�� ���� ��� (-1, 1), ���� �ϴ��� (1, -1)�̴�.
	float x = (float)mouseX * 2 / m_screenWidth - 1.f;
	float y = -(float)mouseY * 2 / m_screenHeight + 1.f;

	// ���� ������ ������ �ʵ��� clamp 
	// ���� ��ǥ���� ����
	float gapX = std::clamp(x, -1.f, 1.f) - prevX;
	float gapY = std::clamp(y, -1.f, 1.f) - prevY;

	prevX = x;
	prevY = y;
	// ī�޶� ���� ȸ��
	//m_camera.MouseRotate(gapX, gapY);
}

LRESULT AppBase::AppProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_MOUSEMOVE:
		//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		
		break;
	case WM_LBUTTONUP:
		// cout << "WM_LBUTTONUP Left mouse button" << endl;

		break;
	case WM_RBUTTONUP:
		// cout << "WM_RBUTTONUP Right mouse button" << endl;
		//OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		// ESC ������ ���α׷� ����
		if (wParam == 27)
			DestroyWindow(m_mainWindow);

		// Ű���尡 ���� ���� ����
		m_keyPressed[wParam] = true;

		//std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		break;
	case WM_KEYUP:
		// Ű���尡 ���̻� �ȴ�����
		m_keyPressed[wParam] = false;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	// �⺻ �޽��� ó�� (Ư�� �޽����� ó������ �ʴ� ���)
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

float AppBase::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}

// ������ �ʱ�ȭ
bool AppBase::InitWindow()
{
	// �� â�� ������ �����͸� �ν��Ͻ� �����Ͷ�� �Ѵ�.
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,	// Window Procedure - ���ø����̼� ���� �Լ��� ���� ������
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"Game Engine", // lpszClassName, L-String
		NULL
	};


	// � ü���� window Ŭ���� ���
	if (!RegisterClassEx(&wc))
	{
		std::cout << "RegisterClassEx() failed.\n";
		return false;
	}

	// ������ â ũ��(�ػ�) - �� �ٸ� ������ ������ â�� ����ϱ� ���� ���
	RECT wr = { 0, 0, m_screenWidth, m_screenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// ������â ����
	// hwnd = window �ڵ�
	m_mainWindow = CreateWindowEx(
		0,						// window style
		wc.lpszClassName,		// window class
		L"Graphics Renderer",	// window text
		WS_OVERLAPPEDWINDOW,	// window style
		// size and position
		300, 150,				// ������ ���� ��� x, y ��ǥ
		wr.right - wr.left,		// ������ ���� ���� �ػ�
		wr.bottom - wr.top,		// ������ ���� ���� �ػ�
		NULL,					// Parent WIndow
		NULL,					// Menu
		wc.hInstance,			// Instance handle
		NULL					// �߰����� App Data
	);

	if (m_mainWindow == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // ���� ����
	}

	ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
	UpdateWindow(m_mainWindow);


	return true;
}

// DirectX 3D �ʱ�ȭ
bool AppBase::InitD3D()
{
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	// flag�� debug ����� �� �ɼ��� �ٵ��� ����
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DirectX3D ���� ����
	const D3D_FEATURE_LEVEL featureLevelArray[2] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	// ������ ��� ������ ���� ����
	D3D_FEATURE_LEVEL featureLevel;

	// Swap-Chain ����
	DXGI_SWAP_CHAIN_DESC sd;
	// �ʱ�ȭ
	ZeroMemory(&sd, sizeof(sd));
	// �ػ� ����
	sd.BufferDesc.Width = m_screenWidth;
	sd.BufferDesc.Height = m_screenHeight;
	// Color data format
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferCount = 2; // ���� ���۸�
	// 1�ʴ� 60�� refresh (60 Frame)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	// RenderTarget�� ������� ���
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_mainWindow; // ����� ������
	sd.Windowed = TRUE; // �׵θ� �ִ� ������ ���
	// ������â ���� ��üȭ�� ��� ���� ����
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // _FLIP_ ��� �� MSAA ������
	// ������ swap-chain�� buffer�� MSAA�� ������� �ʴ´�.
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	// Swap-Chain, device, context ����
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		0,								// IDXGIAdapter Type (default)
		driverType,						// graphic driver type
		0,								// ������ ����̹� ���� ����
		createDeviceFlags,				// ���� Debuging ��� �ɼ� ����
		featureLevelArray,				// Graphic api version
		ARRAYSIZE(featureLevelArray),
		D3D11_SDK_VERSION,				// ��κ� D3D11_SDK_VERSION���� ����
		&sd,							// Swap-Chain ����
		m_swapChain.GetAddressOf(),		// ������ Swap-Chain ����
		m_device.GetAddressOf(),		// ������ Direct3D device ����
		&featureLevel,					// ����� ���� ����
		m_context.GetAddressOf()		// ������ context ����
	)))
	{
		std::cout << "D3D11CreateDeviceAndSwapChain() failed.\n";
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		std::cout << "D3D Feature Level 11 not supported.\n";
		return false;
	}

	// MSAA ���� Ȯ��
	UINT numQualityLevels;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
	if (numQualityLevels <= 0)
	{
		std::cout << "MSAA not supported.\n";
	}


	// Swap-Chain�� Back-Buffer (ȭ�� ���)
	ComPtr<ID3D11Texture2D> backBuffer;
	// ������ Swap-Chain���κ��� Buffer�� ������ BackBuffer�� ����
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (!backBuffer)
	{
		std::cout << "Create BackBuffer RenderTargetVeiw() failed.\n";
		return false;
	}
	// ������ �ÿ� RenderTargetView�� �������� �ϴµ� Swap-Chain�� ���� ���ۿ� �������ϰڴٰ� ����
	m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_backBufferRTV.GetAddressOf());

	// ViewPort ����
	ZeroMemory(&m_screenViewPort, sizeof(D3D11_VIEWPORT)); // �޸� �ʱ�ȭ
	// �������� ȭ�� ũ�� ����
	m_screenViewPort.TopLeftX = 0;
	m_screenViewPort.TopLeftY = 0;
	m_screenViewPort.Width = static_cast<float>(m_screenWidth);
	m_screenViewPort.Height = static_cast<float>(m_screenHeight);
	// depth buffering�� ����ϱ� ���� ����
	m_screenViewPort.MinDepth = 0.0f;
	m_screenViewPort.MaxDepth = 1.0f;

	// ������ Viewport ����
	// Rasterization Stage(RS) - 3���� ������ Vertex�� ȭ�鿡 ���������ִ� ����
	m_context->RSSetViewports(1, &m_screenViewPort);
	
	// Rasterization ��� ����
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // �޸� �ʱ�ȭ
	// ��� �������� �� ���� (WireFrame, Solid etc)
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; // �ﰢ�� �޸� ����
	// �ﰢ�� �ո��� Clock-Wise�� ����
	rastDesc.FrontCounterClockwise = false; 

	// RasterizerState ����
	m_device->CreateRasterizerState(&rastDesc, m_SolidRasterizerState.GetAddressOf());

	// WireFrame �� RasterizerState ����
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	m_device->CreateRasterizerState(&rastDesc, m_WireRasterizerState.GetAddressOf());

	// Depth Buffer �ɼ� ����
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = m_screenWidth;
	depthStencilBufferDesc.Height = m_screenHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	// Depth 24bit / Stencil 8 bit ���
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// ���� Depth�� ����ϰ� MSAA�� ����ϰ� ���� �ʴ�.
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	/*
	// MSAA
	if (numQualityLevels > 0)
	{
		depthStencilBufferDesc.SampleDesc.Count = 4; // �� ���� MultiSamples�� ��
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
	}
	else {
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	*/
	// DepthStencil �޸� ���� ��� ����
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT; // GPU�� �а� ���� ����
	// Depth/Stencil ���۷� ��� ����
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	// �׷��� ī�忡 �޸� �����ϴ� ���̶� device interface�� ���� Texture2D �޸� ����
	if (FAILED(m_device->CreateTexture2D(
		&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf())))
	{
		std::cout << "CreateTexture2D() failed.\n";
	}
	// Shader���� ����ϱ� ���� DepthStencilView ����
	if (FAILED(m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(), 0, &m_depthStencilView)))
	{
		std::cout << "CreateDepthStencilView() failed.\n";
	}

	// DepthStencil ����
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC)); // �޸� �ʱ�ȭ
	// depth �� ������Ʈ �����ϵ��� ����
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	// Depth���� �� ����� �� �׸����� ����
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	// ������ ���� ����
	if (FAILED(m_device->CreateDepthStencilState(
		&depthStencilDesc, m_depthStencilState.GetAddressOf())))
	{
		std::cout << "CreateDepthStencilState() failed.\n";
	}

	return true;
}

// GUI �ʱ�ȭ
bool AppBase::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// ��ũ�� �ػ� �ʱ�ȭ
	io.DisplaySize = ImVec2(static_cast<float>(m_screenWidth), static_cast<float>(m_screenHeight));
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Window Init (window handle)
	if (!ImGui_ImplWin32_Init(m_mainWindow))
		return false;

	// Platform/Renderer backends ����
	if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get()))
		return false;

	return true;
}
