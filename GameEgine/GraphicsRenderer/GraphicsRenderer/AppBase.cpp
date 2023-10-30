#include "AppBase.h"
#include <iostream>
#include <algorithm>

#include <omp.h> // 가속

// 클래스 멤버 함수에서 간접적으로 윈도우 메시지를
// 처리할 수 있도록 선언
AppBase* app = nullptr;

// imgui_impl_wind32에 정의된 메시지 처리 함수
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

// DispatchMessage 함수가 호출 
// (hwnd = window 핸들, uMsg = 메시지 코드, wParam, lParam = 메시지와 관련된 추가 데이터)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return app->AppProc(hwnd, uMsg, wParam, lParam);
}

AppBase::AppBase() : m_screenWidth(1920), m_screenHeight(1080), m_mainWindow(0),
m_screenViewPort(D3D11_VIEWPORT()) { // viewPort 지정 

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
			// GUI 사용
			// GUI 프레임 시작
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// 렌더링할 지 기록시작
			ImGui::Begin("Scene Control");

			// ImGui가 측정해주는 Framerate 출력
			ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
				ImGui::GetIO().Framerate);

			// GUI 관련 업데이트
			UpdateGUI();

			ImGui::End(); // 기록종료
			ImGui::Render(); 
			
			// Game Update - Rendering할 데이터를 업데이트
			Update(ImGui::GetIO().DeltaTime);

			// DirectX Rendering - 업데이트한 데이터로 메모리에 Rendering
			Render();

			// GUI Rendering
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// Update and Render additional Platform Windows
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			// GUI 렌더링 후 Present() 호출
			// 그린 Memory인 Back-buffer와 Front-Buffer를 swap해 화면에 출력
			m_swapChain->Present(1, 0); // swap-chain의 두 버퍼 swap
		}
	}
	return 0;
}

void AppBase::OnMouseMove(int mouseX, int mouseY)
{
	// 윈도우의 마우스 위치 범위는 좌측 상단이 (0, 0), 우측 하단이 (width - 1, height -1)이다.
	// 이걸 NDC 좌표 범위로 변환시켜 사용한다. 
	// NDC 좌표는 좌측 상단 (-1, 1), 우측 하단이 (1, -1)이다.
	float x = (float)mouseX * 2 / m_screenWidth - 1.f;
	float y = -(float)mouseY * 2 / m_screenHeight + 1.f;

	// 범위 밖으로 나가지 않도록 clamp 
	// 이전 좌표와의 차이
	float gapX = std::clamp(x, -1.f, 1.f) - prevX;
	float gapY = std::clamp(y, -1.f, 1.f) - prevY;

	prevX = x;
	prevY = y;
	// 카메라 시점 회전
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
		// ESC 누르면 프로그램 종료
		if (wParam == 27)
			DestroyWindow(m_mainWindow);

		// 키보드가 눌린 상태 저장
		m_keyPressed[wParam] = true;

		//std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		break;
	case WM_KEYUP:
		// 키보드가 더이상 안눌리면
		m_keyPressed[wParam] = false;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	// 기본 메시지 처리 (특정 메시지를 처리하지 않는 경우)
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

float AppBase::GetAspectRatio() const
{
	return static_cast<float>(m_screenWidth) / m_screenHeight;
}

// 윈도우 초기화
bool AppBase::InitWindow()
{
	// 각 창의 고유한 데이터를 인스턴스 데이터라고 한다.
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,	// Window Procedure - 어플리케이션 정의 함수에 대한 포인터
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


	// 운영 체제에 window 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		std::cout << "RegisterClassEx() failed.\n";
		return false;
	}

	// 윈도우 창 크기(해상도) - 툴 바를 제외한 윈도우 창만 계산하기 위해 사용
	RECT wr = { 0, 0, m_screenWidth, m_screenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우창 생성
	// hwnd = window 핸들
	m_mainWindow = CreateWindowEx(
		0,						// window style
		wc.lpszClassName,		// window class
		L"Graphics Renderer",	// window text
		WS_OVERLAPPEDWINDOW,	// window style
		// size and position
		300, 150,				// 윈도우 좌측 상단 x, y 좌표
		wr.right - wr.left,		// 윈도우 가로 방향 해상도
		wr.bottom - wr.top,		// 윈도우 세로 방향 해상도
		NULL,					// Parent WIndow
		NULL,					// Menu
		wc.hInstance,			// Instance handle
		NULL					// 추가적인 App Data
	);

	if (m_mainWindow == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // 강제 종료
	}

	ShowWindow(m_mainWindow, SW_SHOWDEFAULT);
	UpdateWindow(m_mainWindow);


	return true;
}

// DirectX 3D 초기화
bool AppBase::InitD3D()
{
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;

	// flag가 debug 모드일 떄 옵션이 붙도록 설정
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// DirectX3D 버전 설정
	const D3D_FEATURE_LEVEL featureLevelArray[2] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};
	// 실제로 사용 가능한 버전 저장
	D3D_FEATURE_LEVEL featureLevel;

	// Swap-Chain 설정
	DXGI_SWAP_CHAIN_DESC sd;
	// 초기화
	ZeroMemory(&sd, sizeof(sd));
	// 해상도 지정
	sd.BufferDesc.Width = m_screenWidth;
	sd.BufferDesc.Height = m_screenHeight;
	// Color data format
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferCount = 2; // 더블 버퍼링
	// 1초당 60번 refresh (60 Frame)
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	// RenderTarget의 출력으로 사용
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_mainWindow; // 출력할 윈도우
	sd.Windowed = TRUE; // 테두리 있는 윈도우 모드
	// 윈도우창 모드와 전체화면 모드 변경 가능
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // _FLIP_ 사용 시 MSAA 미지원
	// 마지막 swap-chain의 buffer는 MSAA를 사용하지 않는다.
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	// Swap-Chain, device, context 생성
	if (FAILED(D3D11CreateDeviceAndSwapChain(
		0,								// IDXGIAdapter Type (default)
		driverType,						// graphic driver type
		0,								// 별도의 드라이버 사용시 설정
		createDeviceFlags,				// 현재 Debuging 모드 옵션 설정
		featureLevelArray,				// Graphic api version
		ARRAYSIZE(featureLevelArray),
		D3D11_SDK_VERSION,				// 대부분 D3D11_SDK_VERSION으로 설정
		&sd,							// Swap-Chain 설정
		m_swapChain.GetAddressOf(),		// 생성한 Swap-Chain 저장
		m_device.GetAddressOf(),		// 생성한 Direct3D device 저장
		&featureLevel,					// 사용할 버전 저장
		m_context.GetAddressOf()		// 생성한 context 저장
	)))
	{
		std::cout << "D3D11CreateDeviceAndSwapChain() failed.\n";
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		std::cout << "D3D Feature Level 11 not supported.\n";
		return false;
	}

	// MSAA 지원 확인
	UINT numQualityLevels;
	m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
	if (numQualityLevels <= 0)
	{
		std::cout << "MSAA not supported.\n";
	}


	// Swap-Chain의 Back-Buffer (화면 출력)
	ComPtr<ID3D11Texture2D> backBuffer;
	// 생성한 Swap-Chain으로부터 Buffer를 가져와 BackBuffer로 설정
	m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
	if (!backBuffer)
	{
		std::cout << "Create BackBuffer RenderTargetVeiw() failed.\n";
		return false;
	}
	// 렌더링 시에 RenderTargetView에 렌더링을 하는데 Swap-Chain이 가진 버퍼에 렌더링하겠다고 지정
	m_device->CreateRenderTargetView(backBuffer.Get(), NULL, m_backBufferRTV.GetAddressOf());

	// ViewPort 설정
	ZeroMemory(&m_screenViewPort, sizeof(D3D11_VIEWPORT)); // 메모리 초기화
	// 렌더링할 화면 크기 설정
	m_screenViewPort.TopLeftX = 0;
	m_screenViewPort.TopLeftY = 0;
	m_screenViewPort.Width = static_cast<float>(m_screenWidth);
	m_screenViewPort.Height = static_cast<float>(m_screenHeight);
	// depth buffering을 사용하기 위해 설정
	m_screenViewPort.MinDepth = 0.0f;
	m_screenViewPort.MaxDepth = 1.0f;

	// 설정한 Viewport 적용
	// Rasterization Stage(RS) - 3차원 공간의 Vertex를 화면에 투영시켜주는 과정
	m_context->RSSetViewports(1, &m_screenViewPort);
	
	// Rasterization 방법 설정
	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // 메모리 초기화
	// 어떻게 렌더링할 지 설정 (WireFrame, Solid etc)
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK; // 삼각형 뒷면 제외
	// 삼각형 앞면이 Clock-Wise로 설정
	rastDesc.FrontCounterClockwise = false; 

	// RasterizerState 생성
	m_device->CreateRasterizerState(&rastDesc, m_SolidRasterizerState.GetAddressOf());

	// WireFrame 용 RasterizerState 생성
	rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	m_device->CreateRasterizerState(&rastDesc, m_WireRasterizerState.GetAddressOf());

	// Depth Buffer 옵션 설정
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = m_screenWidth;
	depthStencilBufferDesc.Height = m_screenHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	// Depth 24bit / Stencil 8 bit 사용
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	// 현재 Depth만 사용하고 MSAA는 사용하고 있지 않다.
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	/*
	// MSAA
	if (numQualityLevels > 0)
	{
		depthStencilBufferDesc.SampleDesc.Count = 4; // 몇 개를 MultiSamples할 지
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
	}
	else {
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	*/
	// DepthStencil 메모리 공간 사용 설정
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT; // GPU가 읽고 쓰기 가능
	// Depth/Stencil 버퍼로 사용 가능
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	// 그래픽 카드에 메모리 생성하는 것이라 device interface를 통해 Texture2D 메모리 생성
	if (FAILED(m_device->CreateTexture2D(
		&depthStencilBufferDesc, 0, m_depthStencilBuffer.GetAddressOf())))
	{
		std::cout << "CreateTexture2D() failed.\n";
	}
	// Shader에서 사용하기 위해 DepthStencilView 생성
	if (FAILED(m_device->CreateDepthStencilView(
		m_depthStencilBuffer.Get(), 0, &m_depthStencilView)))
	{
		std::cout << "CreateDepthStencilView() failed.\n";
	}

	// DepthStencil 설정
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC)); // 메모리 초기화
	// depth 값 업데이트 가능하도록 설정
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	// Depth값이 더 가까운 걸 그리도록 설정
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	// 생성한 설정 적용
	if (FAILED(m_device->CreateDepthStencilState(
		&depthStencilDesc, m_depthStencilState.GetAddressOf())))
	{
		std::cout << "CreateDepthStencilState() failed.\n";
	}

	return true;
}

// GUI 초기화
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

	// 스크린 해상도 초기화
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

	// Platform/Renderer backends 세팅
	if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get()))
		return false;

	return true;
}
