#include "Scene.h"
#include <queue>


void Scene::RecalculateTree(RECT window)
{	
	std::queue<std::pair<UI::UIObject*, D2D1_RECT_F>> layers;

	D2D1_RECT_F parent = D2D1::RectF(window.left, window.top, window.right, window.bottom);

	layers.push({ m_root,  parent });

	while (!layers.empty())
	{
		std::pair<UI::UIObject*, D2D1_RECT_F> test = layers.front();
		layers.pop();

		auto uiObject = test.first;
		uiObject->Transform.CalculateRect(test.second);
		D2D1_RECT_F result = test.first->Transform.GetRect();

		for (auto child : test.first->m_children)
		{
			layers.push({ child, result });
		}
	}
}


UI::UIObject* SearchTree(UI::UIObject* object, D2D1_POINT_2F mousePoint); //super simple return to later


D2D1_POINT_2F GetCenter(D2D1_RECT_F rect)
{
	return  D2D1::Point2F(
		rect.left + ((rect.right - rect.left) / 2),
		rect.top + ((rect.bottom - rect.top) / 2)
	);
}

UI::UIObject* SearchTree(UI::UIObject* object, D2D1_POINT_2F point) //super simple return to later
{
	if (!object->m_children.empty())
	{
		for (auto child : object->m_children)
		{
			auto hit = SearchTree(child, point);
			if (hit != NULL)
			{
				return hit;
			}
		}
	}

	if (object->m_interaction == NULL)
		return NULL;

	if (object->m_interaction->HitTest(object->Transform, point))
	{
		return object;
	}
	return NULL;
}

void Scene::Resize(HWND handle)
{

}

Scene* Scene::CreateExampleScene()
{
	Scene* scene = new Scene();
	/*UI::UIPanel* background = new UI::UIPanel();
	background->m_anchors = { 0.1f,0.1f,0.9f,0.9f };
	background->m_pivot = { 0.5f,0.5f };
	background->m_sizeDelta = { 0,0 };
	background->m_anchoredOffset = { 0,0 };
	background->m_colour = D2D1::ColorF(D2D1::ColorF::Black);
	background->name = "background";
	scene->m_root = background;

	UI::UIPanel* purple = new UI::UIPanel();
	purple->m_anchors = { 0,0,1,1 };
	purple->m_pivot = { 0.5f,1.0f };
	purple->m_sizeDelta = { -5,-55.0f };
	purple->m_anchoredOffset = { 0,-2.5f };
	purple->m_colour = D2D1::ColorF(D2D1::ColorF::BlueViolet);
	purple->name = "purple";
	background->m_children.push_back(purple);

	UI::UIPanel* blue= new UI::UIPanel();
	blue->m_anchors = { 0.5f,0.5f,0.5f,0.5f };
	blue->m_pivot = { 0.5f,0.5f };
	blue->m_sizeDelta = { 50,50 };
	blue->m_anchoredOffset = { 0,0 };
	blue->m_colour = D2D1::ColorF(D2D1::ColorF::Blue);
	blue->name = "blue";
	purple->m_children.push_back(blue);

	UI::UIPanel* red= new UI::UIPanel();
	red->m_anchors = { 0.1f,0,1,0 };
	red->m_pivot = { 0.5f,0 };
	red->m_sizeDelta = { -40,50 };
	red->m_anchoredOffset = { 0,0 };
	red->m_colour = D2D1::ColorF(D2D1::ColorF::Red);
	red->name = "red";
	background->m_children.push_back(red);

	UI::UIPanel* yellow= new UI::UIPanel();
	yellow->m_anchors = { 0,1,0,1 };
	yellow->m_pivot = { 0,1 };
	yellow->m_sizeDelta = { 100,20 };
	yellow->m_anchoredOffset = { 2.5f,-2.5f };
	yellow->m_colour = D2D1::ColorF(D2D1::ColorF::Yellow);
	yellow->name = "yellow";
	red->m_children.push_back(yellow);*/
	return scene;
}