#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>
#include "MainWindow.h"
#include "Scene.h"
#include "WindowsUtils.h"

UI::UIPanel* CreatePanel(const char* name, UI::UIObject* parent, D2D1::ColorF::Enum colour, D2D1_RECT_F anchors = D2D1::RectF(0, 0, 1, 1), D2D1_RECT_F offset = D2D1::RectF(0, 0, 0, 0));
float DPIScale::scale = 1.0f;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	MainWindow mainWindow;
	if (!mainWindow.Create(L"Main", WS_OVERLAPPEDWINDOW))
		return 0;

	Scene scene; //local scope
	scene.CreateScene(); //test

	//todo remember that all number for offfsrets in dips
	UI::UIPanel* background = CreatePanel("background", nullptr, D2D1::ColorF::Green, D2D1::RectF(0.0, 0.0f, 1.0f, 1.0f), D2D1::RectF(10, 10, 10, 10));
	UI::UIPanel* leftPanel = CreatePanel("left", background, D2D1::ColorF::Blue, D2D1::RectF(0.0, 0.0f, 0.5f, 1.0f), D2D1::RectF(20, 20, 20, 20));
	UI::UIPanel* shortP = CreatePanel("line", leftPanel, D2D1::ColorF::Yellow, D2D1::RectF(0.0, 0.4f, 1.0f, 0.6f), D2D1::RectF(-50, -50, -50, -50));
	UI::UIPanel* thing = CreatePanel("thing", shortP, D2D1::ColorF::Red, D2D1::RectF(0.5, 0.0f, 1.0f, 1.0f), D2D1::RectF(5, 5, 5, 5));
	UI::UIPanel* rightPanel = CreatePanel("right", background, D2D1::ColorF::Blue, D2D1::RectF(0.5, 0.0f, 1.0f, 1.0f));

	UI::UICircle* circle = new UI::UICircle();
	circle->m_anchors = D2D1::RectF(0.0, 0.0f, 1.0f, 1.0f);
	circle->m_offset = D2D1::RectF(0, 0, 0, 0);
	circle->m_colour = D2D1::ColorF(D2D1::ColorF::Orange);
	circle->name = "circle";
	rightPanel->m_children.push_back(circle);

	//UIDebugMouse = create();
	//add mouse to scene
	//mainwindow.mousevents.listen((pos)=> {
	// UIDebugMouse.positon = pos;
	// })


	scene.m_root = background;

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

UI::UIPanel* CreatePanel(const char* name, UI::UIObject* parent, D2D1::ColorF::Enum colour, D2D1_RECT_F anchors, D2D1_RECT_F offset)
{
	UI::UIPanel* panel = new UI::UIPanel();
	panel->m_anchors = anchors;
	panel->m_offset = offset;
	panel->m_colour = D2D1::ColorF(colour);
	panel->name = name;
	if (parent != nullptr) {
		parent->m_children.push_back(panel);
	}
	return panel;
}
