#pragma once
#include <vector>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace UI 
{
	enum ObjectType
	{
		Panel,
		Circle
	};

	struct UIObject
	{
		bool m_anchored;
		D2D1_POINT_2F m_position;
		D2D1_POINT_2F m_size;
		D2D1_RECT_F m_anchors;
		D2D1_RECT_F m_offset;
		virtual ObjectType GetType() = 0;
		std::vector<UIObject*> m_children;
		const char* name;
	};

	struct UIPanel : public UIObject
	{
		ObjectType GetType() { return ObjectType::Panel; }
		D2D1_COLOR_F m_colour;
	};

	struct UICircle : public UIObject
	{
		ObjectType GetType() { return ObjectType::Circle; }
		D2D1_COLOR_F m_colour;
	};
}