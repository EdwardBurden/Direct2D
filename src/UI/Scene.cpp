#include "UI/Scene.h"
#include <queue>

namespace UI
{
	void Scene::RecalculateTree(RECT window)
	{
		std::queue<std::pair<UI::Node*, D2D1_RECT_F>> layers;

		D2D1_RECT_F parent = D2D1::RectF(window.left, window.top, window.right, window.bottom);

		layers.push({ m_root,  parent });

		while (!layers.empty())
		{
			std::pair<UI::Node*, D2D1_RECT_F> test = layers.front();
			layers.pop();

			auto uiObject = test.first;
			uiObject->Transform.CalculateRect(test.second);
			D2D1_RECT_F result = test.first->Transform.GetRect();

			for (auto child : test.first->Children)
			{
				layers.push({ child, result });
			}
		}
	}


	UI::Node* Scene::SearchTree(D2D1_POINT_2F point) //super simple return to later
	{
		return SearchTree(m_root, point);
	}

	UI::Node* Scene::SearchTree(UI::Node* object, D2D1_POINT_2F point) //super simple return to later
	{
		if (!object->Children.empty())
		{
			for (auto child : object->Children)
			{
				auto hit = SearchTree(child, point);
				if (hit != NULL)
				{
					return hit;
				}
			}
		}

		if (object->Collider == NULL)
			return NULL;

		if (object->Collider->HitTest(object->Transform, point))
		{
			return object;
		}
		return NULL;
	}

	void Scene::Resize(HWND handle)
	{

	}
}