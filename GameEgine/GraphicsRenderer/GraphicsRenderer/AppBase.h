#include <Windows.h>

class AppBase {
public:
	AppBase();
	~AppBase();
	// ������ �Է� ����
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevIstance, PWSTR pCmdLine, int nCmdShow);
public:
};