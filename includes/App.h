#pragma once
#include "Window/MainWindow.h"
#include "UI/Renderer.h"
#include "UI/Scene.h"
#include "UI/Node.h"
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
	void OnWindowResize(RECT rect); // listen function
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
	UI::Scene* CreateScene();
	UI::Scene* CreateHierarchyView(UI::Node* target);
	Window::MainWindow* m_window;
	UI::Renderer* m_renderer;
	UI::Scene* m_scene;
	bool m_showDebug;
	UI::Scene* m_debugHierarchy;
	UI::Node* m_pressTarget;
	bool m_dragging; 
	POINT m_pressStart;
};
