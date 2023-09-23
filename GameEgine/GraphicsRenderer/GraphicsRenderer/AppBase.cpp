#include "AppBase.h"
#include <iostream>

// 전역 변수로 선언해 이벤트 등의 메시지를 처리
AppBase* appBase = nullptr;

// imgui_impl_wind32에 정의된 메시지 처리 함수
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);


// DispatchMessage 함수가 호출 
// (hwnd = window 핸들, uMsg = 메시지 코드, wParam, lParam = 메시지와 관련된 추가 데이터)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return appBase->MsgProc(hwnd, uMsg, wParam, lParam);
}


AppBase::AppBase() : m_screenWidth(1920), m_screenHeight(1080), m_mainWindow(0),
m_screenViewPort(D3D11_VIEWPORT()) // viewPort 지정
{
	appBase = this;
}

AppBase::~AppBase()
{
	appBase = nullptr;

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
			
			// Game Update
			Update(ImGui::GetIO().DeltaTime);

			// DirectX Rendering
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
			m_swapChain->Present(1, 0); // swap-chain의 두 버퍼 swap
		}
	}
	return 0;
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
	m_device->CreateRasterizerState(&rastDesc, m_rasterizerState.GetAddressOf());

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

void AppBase::CreateVSAndInputLayout(const std::wstring& filename, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
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
	m_device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),	// Compile한 Shader
		shaderBlob->GetBufferSize(),	// Compile한 Shader의 크기
		NULL,							// ClassLinkage 인터페이스 포인터
		&vertexShader					// 생성한 shader 할당
	);

	// Input Layout 생성
	m_device->CreateInputLayout(
		inputElements.data(),			// InputLayout 데이터 형식  배열
		UINT(inputElements.size()),		// Input-Elements 배열 크기
		shaderBlob->GetBufferPointer(),	// Compile한 Shader
		shaderBlob->GetBufferSize(),	// Compile한 Shader 크기
		&inputLayout					// 생성한 InputLayout 할당
	);
}

void AppBase::CreatePS(const std::wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
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

	m_device->CreatePixelShader(
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		NULL, &pixelShader);
}

void AppBase::CreateIndexBuffer(const std::vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
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

	m_device->CreateBuffer(&bDesc, &indexBufferData, indexBuffer.GetAddressOf());
}


LRESULT CALLBACK AppBase::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		// cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
		break;
	case WM_LBUTTONUP:
		// cout << "WM_LBUTTONUP Left mouse button" << endl;
		break;
	case WM_RBUTTONUP:
		// cout << "WM_RBUTTONUP Right mouse button" << endl;
		break;
	case WM_KEYDOWN:
		// cout << "WM_KEYDOWN " << (int)wParam << endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	// 기본 메시지 처리 (특정 메시지를 처리하지 않는 경우)
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}