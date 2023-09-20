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

	// ������ â ���
	const wchar_t CLASS_NAME[] = L"Game Engine";

	WNDCLASSEX wc = {
	};

	wc.lpfnWndProc = WindowProc;
	// �ν��Ͻ��� ���� �ڵ��̰ų� ��⿡ ���� �ڵ�.
	// �� ���� ����� �޸𸮿� �ε�� �� ���� ���� �Ǵ� exe�� �ĺ��Ѵ�.
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassEx(&wc);

	// ������ â ũ��(�ػ�) - �� �ٸ� ������ ������ â�� ����ϱ� ���� ���
	const int width = 1920, height = 1080;
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// ������â ����
	HWND hwnd = CreateWindowEx(
		0,						// window style
		CLASS_NAME,				// window class
		L"Graphics Renderer",	// window text
		WS_OVERLAPPEDWINDOW,	// window style
		// size and position
		1250, 1250,				// ������ ���� ��� x, y ��ǥ
		wr.right - wr.left,		// ������ ���� ���� �ػ�
		wr.bottom - wr.top,		// ������ ���� ���� �ػ�
		NULL,					// Parent WIndow
		NULL,					// Menu
		wc.hInstance,				// Instance handle
		NULL					// �߰����� App Data
	);

	if (hwnd == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // ���� ����
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