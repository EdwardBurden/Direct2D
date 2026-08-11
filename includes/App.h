#pragma once
#include "MainWindow.h"
#include "Renderer.h"
#include "Scene.h"
#include "Input.h"

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

	//temp, still thinkging
	void OnMouseClick();

	MainWindow* m_window;
	Renderer* m_renderer;
	Scene* m_scene;
	Input* m_input;

	//UI::UIPanel m_hierarchyPanel; //debug
//	UI::UIPanel m_transformPanel; //debug
};
