#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "App.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	App app;
	auto result = app.StartUp(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	if (result != 0)
	{
		return result;
	}
	while (app.Alive)
	{
		app.Update();
	}
	app.Shutdown();

	return 0;
}
