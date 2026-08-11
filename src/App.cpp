#include "App.h"
#include <iostream>
#include <WindowsUtils.h>

float DPIScale::scale = 1.0f;

int App::StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	m_input = new Input();

	m_scene = Scene::CreateExampleScene();

	m_renderer = new Renderer();


	m_window = new MainWindow();


	//Callbacks
	m_window->MouseInput = [this](UINT msg , int x, int y, DWORD flags) //all mouse input
		{
			m_input->OnMouseInput(msg, x, y, flags);
		};

	m_window->WindowResize = [this](D2D1_SIZE_U rect)
		{
			OnWindowResize(rect);
		};

	/*m_input.MouseClick = [this](UINT msg, int x, int y, DWORD flags) //only click
		{
			OnMouseClick(msg, x, y, flags);
		};*/


	if (!m_window->Create(L"Main", WS_OVERLAPPEDWINDOW))
		return 1;	
	
	m_renderer->Create(m_window->GetWindowHandle());
	ShowWindow(m_window->GetWindowHandle(), nCmdShow);
	return 0;
}

bool App::Update()
{
	m_window->PollMessages(); //will fire messages to recievers
	//m_input.GetEvent();


	RenderLoop();

	return true;
}

void App::OnMouseClick()
{
	//auto uioobject = m_scene.Hittest();
	// uiobject.click();
}

void App::RenderLoop() 
{
	RECT rect;
	GetClientRect(m_window->GetWindowHandle(), &rect);
	m_scene->RecalculateTree(rect);

	m_window->BeginWindowPaint();
	m_renderer->BeginRender(m_window->GetWindowHandle());
	m_renderer->RenderUIObject(m_scene->m_root); // whole scene
	m_renderer->EndRender();
	m_window->EndWindowPaint();
}

void App::OnWindowResize(D2D1_SIZE_U rect)
{
	std::cout << "Resize";
	m_scene->Resize(m_window->GetWindowHandle());
	m_renderer->Resize(m_window->GetWindowHandle());
	RenderLoop();
}

void App::Shutdown()
{
}
