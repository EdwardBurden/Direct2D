#pragma once
#include <vector>
#include <functional>
#include <string>
#include "UI/Graphic.h"
#include "UI/RectTransform.h"

namespace UI
{
	class Collider
	{
	public:
		virtual ~Collider() = default;
		bool HitTest(const RectTransform& transform, D2D1_POINT_2F test)
		{
			auto rect = transform.GetRect();
			return
				test.x >= rect.left &&
				test.x <= rect.right &&
				test.y >= rect.top &&
				test.y <= rect.bottom;
		}

		std::function<void(POINT p)> OnPointerDown;
		std::function<void(POINT p)> OnPointerUp;
		std::function<void(POINT p)> OnPointerDrag;
	};

	class Node
	{
	public:
		const wchar_t* DebugName;
		RectTransform Transform;
		Graphic* Graphic;
		Collider* Collider;
		Node* Parent;
		std::vector<Node*> Children;

		Node(const Node& node) = delete;
		Node& operator=(const Node& node) = delete;

		Node(const wchar_t* name, Node* parent) : DebugName(name), Parent(parent), Graphic(nullptr), Collider(nullptr)
		{
			if (Parent == nullptr)
				return;

			Parent->Children.push_back(this);
		}
		
		~Node()
		{
			delete Graphic;
			delete Collider;

			for (auto child : Children)
			{
				delete child;
			}
			Children.clear();
		}
	};
}