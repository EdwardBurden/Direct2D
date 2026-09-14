#pragma once
#include <vector>
#include <functional>
#include <string>
#include "UIRender.h"

namespace UI
{
	struct UIInteractionCollider
	{
		bool HitTest(const UIRectTransform& transform, D2D1_POINT_2F test)
		{
			auto rect = transform.GetRect();
			return
				test.x >= rect.left &&
				test.x <= rect.right &&
				test.y >= rect.top &&
				test.y <= rect.bottom; //simple version for now
		}

		std::function<void(POINT p)> OnPointerDown;
		std::function<void(POINT p)> OnPointerUp;
		std::function<void(POINT p)> OnPointerDrag;
	};

	struct UIObject
	{
		const wchar_t* DebugName;
		UIRectTransform Transform;	//never null
		UIBaseRender* Render;//nullable
		UIInteractionCollider* Collider;
		UIObject* Parent;
		std::vector<UIObject*> Children;
		UIObject(const wchar_t* name, UIObject* parent) : DebugName(name), Parent(parent), Render(NULL), Collider(NULL)
		{
			if (Parent == NULL)
				return;

			Parent->Children.push_back(this);
		}
		
		~UIObject()
		{
			delete Render;
			delete Collider;
			for (auto child : Children)
			{
				delete child;
			}
			Children.clear();
			if (Parent == NULL)
				return;
			auto it = std::find(Parent->Children.begin(), Parent->Children.end(), this);
			Parent->Children.erase(it);
		}
	};
}