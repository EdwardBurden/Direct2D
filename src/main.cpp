#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>
#include "MainWindow.h"
#include "Scene.h"
#include "WindowsUtils.h"

float DPIScale::scale = 1.0f;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow mainWindow;
	if (!mainWindow.Create(L"Main", WS_OVERLAPPEDWINDOW))
		return 0;

	Scene scene; //local scope
	scene.CreateScene(); //test

	UI::UIPanel background;
	background.m_anchors = { 0.1f,0.1f,0.9f,0.9f };
	background.m_pivot = { 0.5f,0.5f };
	background.m_sizeDelta = { 0,0 };
	background.m_anchoredOffset = { 0,0 };
	background.m_colour = D2D1::ColorF(D2D1::ColorF::Black);
	background.name = "background";
	scene.m_root = &background;

	UI::UIPanel purple;
	purple.m_anchors = { 0,0,1,1 };
	purple.m_pivot = { 0.5f,1.0f };
	purple.m_sizeDelta = { -5,-55.0f };
	purple.m_anchoredOffset = { 0,-2.5f };
	purple.m_colour = D2D1::ColorF(D2D1::ColorF::BlueViolet);
	purple.name = "purple";
	background.m_children.push_back(&purple);

	UI::UIPanel blue;
	blue.m_anchors = { 0.5f,0.5f,0.5f,0.5f };
	blue.m_pivot = { 0.5f,0.5f };
	blue.m_sizeDelta = { 50,50 };
	blue.m_anchoredOffset = { 0,0 };
	blue.m_colour = D2D1::ColorF(D2D1::ColorF::Blue);
	blue.name = "blue";
	purple.m_children.push_back(&blue);

	UI::UIPanel red;
	red.m_anchors = { 0.1f,0,1,0 };
	red.m_pivot = { 0.5f,0 };
	red.m_sizeDelta = { -40,50 };
	red.m_anchoredOffset = { 0,0 };
	red.m_colour = D2D1::ColorF(D2D1::ColorF::Red);
	red.name = "red";
	background.m_children.push_back(&red);

	UI::UIPanel yellow;
	yellow.m_anchors = { 0,1,0,1 };
	yellow.m_pivot = { 0,1 };
	yellow.m_sizeDelta = { 100,20 };
	yellow.m_anchoredOffset = { 2.5f,-2.5f };
	yellow.m_colour = D2D1::ColorF(D2D1::ColorF::Yellow);
	yellow.name = "yellow";
	red.m_children.push_back(&yellow);

	mainWindow.SetScene(&scene);
	ShowWindow(mainWindow.GetWindowHandle(), nCmdShow);

	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		mainWindow.RenderWindow();
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
