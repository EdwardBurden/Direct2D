#include "Scene.h"
#include <queue>

UI::UIObject* SearchTree(UI::UIObject* object, D2D1_POINT_2F mousePoint); //super simple return to later

D2D1_POINT_2F GetCenter(D2D1_RECT_F rect)
{
	return  D2D1::Point2F(
		rect.left + ((rect.right - rect.left) / 2),
		rect.top + ((rect.bottom - rect.top) / 2)
	);
}

HRESULT Scene::CreateScene()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (FAILED(hr))
		return -1;
	return 0;
}

void Scene::DrawScene(HWND handle, D2D1_POINT_2F mousePoint)
{
	HRESULT hr = CreateGraphicsResources(handle);
	if (FAILED(hr))
		return;

	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));
	//setup
	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_RECT_F rectF = D2D1::RectF(rect.left, rect.top, rect.right, rect.bottom);

	DrawSceneTree(m_root, rectF);
	m_debugMouse.point = mousePoint;
	m_brush->SetColor(D2D1::ColorF(m_debugMouseColour));
	m_renderTarget->FillEllipse(&m_debugMouse, m_brush);
	//clean
	hr = m_renderTarget->EndDraw();
	if (FAILED(hr))
	{
		//clean up for next time
	}
}

void Scene::OnMouseClick(HWND handle, D2D1_POINT_2F mousePoint)
{
	UI::UIObject* clicked = SearchTree(m_root, mousePoint);
	if (clicked == NULL)
		return; //todo

	if (clicked == m_selected)
	{
		//do nothing
	}
	else
	{
		if (m_selected != NULL) {
			m_selected->selected = false;
		}
		m_selected = clicked;
		m_selected->selected = true;
	}

}

UI::UIObject* SearchTree(UI::UIObject* object, D2D1_POINT_2F mousePoint) //super simple return to later
{
	if (!object->m_children.empty())
	{
		for (auto child : object->m_children)
		{
			auto hit = SearchTree(child, mousePoint);
			if (hit != NULL)
			{
				return hit;
			}
		}
	}

	if (object->HitTest(mousePoint))
	{
		return object;
	}
	return NULL;
}


void Scene::DrawSceneTree(UI::UIObject* object, D2D1_RECT_F parent)
{
	std::queue<std::pair<UI::UIObject*, D2D1_RECT_F>> layers;
	layers.push({ object,  parent });

	while (!layers.empty())
	{
		std::pair<UI::UIObject*, D2D1_RECT_F> test = layers.front();
		layers.pop();

		D2D1_RECT_F result = test.first->CalculateRect(test.second);
		m_renderTarget->PushAxisAlignedClip(result, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		DrawObject(test.first, result);

		for (auto child : test.first->m_children)
		{
			layers.push({ child, result });
		}
		m_renderTarget->PopAxisAlignedClip();
	}
}

void Scene::DrawObject(UI::UIObject* object, D2D1_RECT_F rect) //to be obselete
{
	auto type = object->GetType();
	if (type == UI::ObjectType::Panel)
	{
		UI::UIPanel* panel = static_cast<UI::UIPanel*>(object);
		m_brush->SetColor(panel->m_colour);

		if (panel->selected)
		{
			m_brush->SetColor(D2D1::ColorF(D2D1::ColorF::Beige));
		}

		m_renderTarget->FillRectangle(&rect, m_brush);
	}
}

HRESULT Scene::CreateGraphicsResources(HWND handle)
{
	if (m_renderTarget != NULL)
		return S_OK;

	HRESULT hr = NULL;
	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom); // width and height from top left 
	hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(handle, size),
		&m_renderTarget
	);
	if (FAILED(hr))
		return S_FALSE;

	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 0), &m_brush);
	if (FAILED(hr))
		return S_FALSE;

	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}


void Scene::Resize(HWND handle)
{
	if (m_renderTarget == NULL)
		return;

	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	m_renderTarget->Resize(size);
}

//0,non:1,down,2:up.3:drag
void Scene::SetMouseState(int state)
{
	switch (state)
	{
	case 0: m_debugMouseColour = D2D1::ColorF::Black;
		break;
	case 1: m_debugMouseColour = D2D1::ColorF::Aqua;
		break;
	case 2: m_debugMouseColour = D2D1::ColorF::Red;
		break;
	case 3: m_debugMouseColour = D2D1::ColorF::Yellow;
		break;
	default:
		break;
	}

}