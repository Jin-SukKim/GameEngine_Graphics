#include "AppBase.h"
#include <iostream>

int main()
{
	AppBase app;
	IMGUI_VERSION;
	if (!app.Initialize())
	{
		std::cout << "Initialization failed.\n";
		return -1;
	}

	return app.Run();
}