#include <iostream>
#include <windows.h>

// DispatchMessage 함수가 호출 
// (hwnd = window 핸들, uMsg = 메시지 코드, wParam, lParam = 메시지와 관련된 추가 데이터)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIstance, LPSTR lpCmdLine, int nShowCmd) {


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

	// 인스턴스에 대한 핸들이거나 모듈에 대한 핸들.
	// 이 값을 사용해 메모리에 로드될 때 실행 파일 또는 exe를 식별한다.
	wc.hInstance = hInstance;
	// 운영 체제에 window 클래스 등록
	RegisterClassEx(&wc);

	// 윈도우 창 크기(해상도) - 툴 바를 제외한 윈도우 창만 계산하기 위해 사용
	const int width = 1920, height = 1080;
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// 윈도우창 생성
	// hwnd = window 핸들
	HWND hwnd = CreateWindowEx(
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
		wc.hInstance,				// Instance handle
		NULL					// 추가적인 App Data
	);

	if (hwnd == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // 강제 종료
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Run Message Loop.
	MSG msg = {};
	// 창을 생성한 각 쓰레드에 대해 OS는 Window MSG에 대한 큐를 생성한다.
	// 이 큐는 GetMessage, PeekMessage 함수로 Message를 가져올 수 있다.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		// 키 입력을 문자로 변환
		TranslateMessage(&msg);
		// OS에 메시지의 대상인 Window의 WindProc을 호출
		// 즉, OS는 Window Table에서 Window Handle을 조회하고,
		// Window와 연결된 함수 포인터를 찾은 다음 함수를 호출한다.
		DispatchMessage(&msg);
	}

	return 0;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		DestroyWindow(hwnd);
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

	// 기본 메시지 처리 (특정 메시지를 처리하지 않는 경우)
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}