#include "App.h"
#include <iostream>
#include <WindowsUtils.h>
#include <stack>

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

//void App::OnRightMouseClick(POINT p)
//{
//DELTE SELECTED RECT
//}

void App::OnMouseClick(POINT p)
{
	auto point = D2D1::Point2F(p.x, p.y);
	m_pressTarget = m_scene->SearchTree(point);

	m_showDebug = m_pressTarget != NULL;

	if (m_debugHierarchy != NULL)
	{
		delete m_debugHierarchy;
		m_debugHierarchy = NULL;
	}
	if (m_showDebug)
	{
		m_debugHierarchy = CreateHierarchyView(m_pressTarget);
	}
	/*
	if (obj != NULL)
	{
		if (obj->m_interaction != NULL && obj->m_interaction->OnPointerDown)
		{
			obj->m_interaction->OnPointerDown(p); //if they can be interacted?
		}
	}
	*/
	//then debug stuff here
//	m_debugSelection = obj;
}

void App::OnDrag(POINT p)
{
	/*
	auto point = D2D1::Point2F(p.x, p.y);
	UI::UIObject* obj = m_scene->SearchTree(point);
	if (obj != NULL)
	{
		if (obj->m_interaction != NULL && obj->m_interaction->OnPointerDrag)
		{
			obj->m_interaction->OnPointerDrag(p); //if they can be interacted?
		}
	}
	*/
}

void App::OnRelease(POINT p)
{


}

void App::RenderLoop()
{
	RECT rect;
	GetClientRect(m_window->GetWindowHandle(), &rect);

	if (m_showDebug)
	{
		//m_scene->m_root->Transform.AnchoredOffset.x = -100;
		m_scene->m_root->Transform.SizeDelta.x = -200;
	}
	else
	{
		//m_scene->m_root->Transform.AnchoredOffset.x = 0;
		m_scene->m_root->Transform.SizeDelta.x = 0;
	}

	m_scene->RecalculateTree(rect);

	m_window->BeginWindowPaint();
	m_renderer->BeginRender(m_window->GetWindowHandle());
	m_renderer->RenderUIObject(m_scene->m_root); // whole scene




	//debug
	if (m_showDebug)
	{
		Debug_DrawHierarchy();
		Debug_DrawGizmos();
		//
	}

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
	RECT rect;
	GetClientRect(m_window->GetWindowHandle(), &rect);
	m_debugHierarchy->RecalculateTree(rect);
	m_renderer->RenderUIObject(m_debugHierarchy->m_root); // whole scene


	//TODO THIS IS THE FOCUS, can be simple to begin with, just the selected items info
}


void App::Debug_DrawGizmos()
{
	//TODO THIS IS THE FOCUS, just some circles for now
}


Scene* App::CreateScene()
{
	Scene* scene = new Scene();
	UI::UIObject* root = UI::CreateBackground();
	scene->m_root = root;

	auto header = UI::CreatePanel(root, L"Header", D2D1::ColorF::Blue, { 0,0,1,0 }, { 0, 100 });
	auto headerInner = UI::CreatePanel(header, L"Header Inner", D2D1::ColorF::BlueViolet, { 0,0,1,1 }, { -10, -10 }, {5,5});
	auto headerTop = UI::CreatePanel(headerInner, L"Header Top", D2D1::ColorF::DodgerBlue, { 0,0,1,0.5 });
	auto headerBottom = UI::CreatePanel(headerInner, L"Header Bottom", D2D1::ColorF::DarkBlue, { 0,0.5,1,1 });
	auto button1 = UI::CreatePanel(headerTop, L"Button 1", D2D1::ColorF::DarkSlateBlue, { 0,0,0,0 }, { 40,40 });
	auto button2 = UI::CreatePanel(headerTop, L"Button 2", D2D1::ColorF::CadetBlue, { 0,0,0,0 }, { 40,40 }, {50,0});

	auto button3 = UI::CreatePanel(headerBottom, L"Button 3", D2D1::ColorF::CornflowerBlue, { 0,0,0,0 }, { 40,40 });
	auto button4 = UI::CreatePanel(headerBottom, L"Button 4", D2D1::ColorF::DeepSkyBlue, { 0,0,0,0 }, { 40,40 }, { 50,0 });


	auto body = UI::CreatePanel(root, L"Body", D2D1::ColorF::DarkRed, { 0,0,1,1 }, { 0,-100 }, { 0,0 }, {0,1});
	auto p1 = UI::CreatePanel(body, L"Top left", D2D1::ColorF::IndianRed, { 0,0,0.5,0.5 });
	auto p2 = UI::CreatePanel(body, L"Top Right", D2D1::ColorF::MediumVioletRed, { 0.5,0,1,0.5 });
	auto p3 = UI::CreatePanel(body, L"Bottom left", D2D1::ColorF::PaleVioletRed, { 0,0.5,0.5,1 });
	auto p4 = UI::CreatePanel(body, L"Bottom Right", D2D1::ColorF::OrangeRed, { 0.5,0.5, 1,1 });


	return scene;
}


Scene* App::CreateHierarchyView(UI::UIObject* target)
{
	Scene* scene = new Scene();
	UI::UIObject* root = new UI::UIObject(L"Root", NULL);
	scene->m_root = root;
	root->Transform.Anchors = { 1,0,1,1 };
	root->Transform.Pivot = { 1,1 };
	root->Transform.SizeDelta = { 200,0 };
	root->Transform.AnchoredOffset = { 0,0 };
	root->Render = new UI::UISquareRender(D2D1::ColorF::Green);
	std::stack<UI::UIObject*> hierarchy;
	auto t = target;
	do
	{
		hierarchy.push(t);
		t = t->Parent;
	} while (t != NULL);

	int height = 0;;
	int indent = 0;
	while (!hierarchy.empty())
	{
		UI::UIObject* top = hierarchy.top();
		auto p1 = UI::CreateHierarchyPanel(root, top->DebugName, D2D1::ColorF::LightYellow);
		p1->Transform.AnchoredOffset.y = height;
		p1->Transform.SizeDelta.x = -indent;
		height += 20;
		indent += 5;
		hierarchy.pop();
	}
	return scene;
}

static  void thing(UI::UIObject* obj)
{
	std::cout << "Click";

}
