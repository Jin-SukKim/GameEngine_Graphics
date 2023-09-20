#include <Windows.h>

class AppBase {
public:
	AppBase();
	~AppBase();
	// 윈도우 입력 전달
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevIstance, PWSTR pCmdLine, int nCmdShow);
public:
};