#include "AppBase.h"
#include <iostream>
#include <memory>

std::unique_ptr<AppBase> app;

AppBase::AppBase()
{
	app = std::make_unique<AppBase>();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return app->MsgProc(hwnd, uMsg, wParam, lParam);
};

int WINAPI AppBase::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevIstance, PWSTR pCmdLine, int nCmdShow) {

	// 윈도우 창 등록
	const wchar_t CLASS_NAME[] = L"Game Engine";

	WNDCLASSEX wc = {
	};

	wc.lpfnWndProc = WindowProc;
	// 인스턴스에 대한 핸들이거나 모듈에 대한 핸들.
	// 이 값을 사용해 메모리에 로드될 때 실행 파일 또는 exe를 식별한다.
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassEx(&wc);

	// 윈도우 창 크기(해상도) - 툴 바를 제외한 윈도우 창만 계산하기 위해 사용
	const int width = 1920, height = 1080;
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우창 생성
	HWND hwnd = CreateWindowEx(
		0,						// window style
		CLASS_NAME,				// window class
		L"Graphics Renderer",	// window text
		WS_OVERLAPPEDWINDOW,	// window style
		// size and position
		1250, 1250,				// 윈도우 좌측 상단 x, y 좌표
		wr.right - wr.left,		// 윈도우 가로 방향 해상도
		wr.bottom - wr.top,		// 윈도우 세로 방향 해상도
		NULL,					// Parent WIndow
		NULL,					// Menu
		wc.hInstance,				// Instance handle
		NULL					// 추가적인 App Data
	);

	if (hwnd == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // 강제 종료
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);

	// Run Message Loop.
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
};

LRESULT CALLBACK AppBase::MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}