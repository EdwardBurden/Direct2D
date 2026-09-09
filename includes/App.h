#pragma once
#include "MainWindow.h"
#include "Renderer.h"
#include "Scene.h"

class App
{
public:
	App() :
		m_window(NULL),
		m_renderer(NULL),
		m_scene(NULL),
		Alive(true)
	{
	}


	int StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
	void OnWindowResize(D2D1_SIZE_U rect); // listen function
	bool Update();
	void Shutdown();
	bool Alive;
private:
	void RenderLoop();
	void OnMouseClick(POINT p);
	void OnDrag(POINT p);
	void OnRelease(POINT p);

	void Debug_DrawHierarchy();
	void Debug_DrawGizmos();
	Scene* CreateScene();
	MainWindow* m_window;
	Renderer* m_renderer;
	Scene* m_scene;


	///todo move later
	UI::UIObject* m_hoverTarget; //thing being hoverred
	UI::UIObject* m_pressTarget; // object that got the press down event
	UI::UIObject* m_dragTraget; //thing last clicked
	bool m_dragging; //is the user dragging
	POINT m_pressStart; //where clickdown happended
};
