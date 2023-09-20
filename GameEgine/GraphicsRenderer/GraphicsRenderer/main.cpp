#include <iostream>
#include <windows.h>

// DispatchMessage �Լ��� ȣ�� 
// (hwnd = window �ڵ�, uMsg = �޽��� �ڵ�, wParam, lParam = �޽����� ���õ� �߰� ������)
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIstance, LPSTR lpCmdLine, int nShowCmd) {


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

	// �ν��Ͻ��� ���� �ڵ��̰ų� ��⿡ ���� �ڵ�.
	// �� ���� ����� �޸𸮿� �ε�� �� ���� ���� �Ǵ� exe�� �ĺ��Ѵ�.
	wc.hInstance = hInstance;
	// � ü���� window Ŭ���� ���
	RegisterClassEx(&wc);

	// ������ â ũ��(�ػ�) - �� �ٸ� ������ ������ â�� ����ϱ� ���� ���
	const int width = 1920, height = 1080;
	RECT wr = { 0, 0, width, height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// ������â ����
	// hwnd = window �ڵ�
	HWND hwnd = CreateWindowEx(
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
		wc.hInstance,				// Instance handle
		NULL					// �߰����� App Data
	);

	if (hwnd == NULL)
	{
		std::cout << "Failed to create window.\n";
		exit(-1); // ���� ����
	}

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	// Run Message Loop.
	MSG msg = {};
	// â�� ������ �� �����忡 ���� OS�� Window MSG�� ���� ť�� �����Ѵ�.
	// �� ť�� GetMessage, PeekMessage �Լ��� Message�� ������ �� �ִ�.
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		// Ű �Է��� ���ڷ� ��ȯ
		TranslateMessage(&msg);
		// OS�� �޽����� ����� Window�� WindProc�� ȣ��
		// ��, OS�� Window Table���� Window Handle�� ��ȸ�ϰ�,
		// Window�� ����� �Լ� �����͸� ã�� ���� �Լ��� ȣ���Ѵ�.
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

	// �⺻ �޽��� ó�� (Ư�� �޽����� ó������ �ʴ� ���)
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}