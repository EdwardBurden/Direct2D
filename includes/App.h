#pragma once
#include "MainWindow.h"
#include "Renderer.h"
#include "Scene.h"
#include "UI/UIObject.h"
#include "UI/UIUtils.h"

class App
{
public:
	App() :
		m_window(NULL),
		m_renderer(NULL),
		m_scene(NULL),
		Alive(true),
		m_showDebug(false),
		m_debugHierarchy(NULL)
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
	Scene* CreateHierarchyView(UI::UIObject* target);
	MainWindow* m_window;
	Renderer* m_renderer;
	Scene* m_scene;
	bool m_showDebug;
	Scene* m_debugHierarchy;
	UI::UIObject* m_pressTarget; 
	bool m_dragging; 
	POINT m_pressStart;
};
