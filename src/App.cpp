#include "App.h"
#include <iostream>
#include <WindowsUtils.h>

float DPIScale::scale = 1.0f;
static  void thing(UI::UIObject* obj);


int App::StartUp(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	m_scene = CreateScene();

	m_renderer = new Renderer();


	m_window = new MainWindow();


	//Callbacks
	m_window->ClickEvent = [this](POINT p) { OnMouseClick(p); };
	m_window->DraggingEvent = [this](POINT p) { OnDrag(p); };
	m_window->UnClickEvent = [this](POINT p) { OnRelease(p); };
	m_window->WindowResize = [this](D2D1_SIZE_U rect) {	OnWindowResize(rect); };

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

void App::OnMouseClick(POINT p)
{
	auto point = D2D1::Point2F(p.x, p.y);
	UI::UIObject* obj = m_scene->SearchTree(point);

	if (obj != NULL)
	{
		if (obj->m_interaction != NULL && obj->m_interaction->OnPointerDown)
		{
			obj->m_interaction->OnPointerDown(p); //if they can be interacted?
		}
	}
	//then debug stuff here
//	m_debugSelection = obj;
}

void App::OnDrag(POINT p)
{
	auto point = D2D1::Point2F(p.x, p.y);
	UI::UIObject* obj = m_scene->SearchTree(point);

	if (obj != NULL)
	{
		if (obj->m_interaction != NULL && obj->m_interaction->OnPointerDrag)
		{
			obj->m_interaction->OnPointerDrag(p); //if they can be interacted?
		}
	}
}

void App::OnRelease(POINT p)
{


}

void App::RenderLoop()
{
	RECT rect;
	GetClientRect(m_window->GetWindowHandle(), &rect);
	m_scene->RecalculateTree(rect);

	m_window->BeginWindowPaint();
	m_renderer->BeginRender(m_window->GetWindowHandle());
	m_renderer->RenderUIObject(m_scene->m_root); // whole scene




	//debug
	Debug_DrawHierarchy();
	Debug_DrawGizmos();
	//


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

void App::Debug_DrawHierarchy()
{

}


void App::Debug_DrawGizmos()
{

}


Scene* App::CreateScene()
{
	Scene* scene = new Scene();
	UI::UIObject* root = UI::Createbackground();
	root->Render = NULL;
	root->m_interaction = NULL;
	scene->m_root = root;

	auto obj = UI::CreateExampleButton("example", thing);
	obj->Transform.Anchors = { 0.4,0, 0.6, 1 };
//	root->m_children.push_back(obj);


	UI::UIObject* test = UI::Createbackground();
	test->Render = NULL;
	test->m_interaction = NULL;
	test->Transform.Anchors = D2D1::RectF(0.4, 0.3, 0.1, 0.1);
	root->m_children.push_back(test);

	auto drag = UI::CreateDraggableExample("drag", {0.1,0.1,0.1,0.1} , {200,50});
	test->m_children.push_back(drag);

	return scene;
}

static  void thing(UI::UIObject* obj)
{
	std::cout << "Click";

}
