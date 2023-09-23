#include "Graphics.h"
#include <iostream>

int main()
{
	Graphics app;
	if (!app.Initialize())
	{
		std::cout << "Initialization failed.\n";
		return -1;
	}

	return app.Run();
}