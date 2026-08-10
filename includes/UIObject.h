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
		D2D1_RECT_F m_anchors;
		D2D1_POINT_2F m_pivot;
		D2D1_POINT_2F m_anchoredOffset;
		D2D1_POINT_2F m_sizeDelta;
		virtual ObjectType GetType() = 0;
		std::vector<UIObject*> m_children;
		const char* name;
		bool selected = false;

		inline D2D1_RECT_F CalculateRect(D2D1_RECT_F parent)
		{
			D2D1_POINT_2F parentSize = { parent.right - parent.left , parent.bottom - parent.top };

			D2D1_RECT_F area;
			area.left = parent.left + (m_anchors.left * parentSize.x);
			area.right = parent.left + (m_anchors.right * parentSize.x);
			area.top = parent.top + (m_anchors.top * parentSize.y);
			area.bottom = parent.top + (m_anchors.bottom * parentSize.y);

			D2D1_POINT_2F size;
			size.x = (area.right - area.left) + m_sizeDelta.x;
			size.y = (area.bottom - area.top) + m_sizeDelta.y;

			D2D1_POINT_2F pivotPoint;
			pivotPoint.x = (area.left + ((area.right - area.left) * m_pivot.x)) + m_anchoredOffset.x;
			pivotPoint.y = (area.top + ((area.bottom - area.top) * m_pivot.y)) + m_anchoredOffset.y;

			m_lastFrameRect.left = pivotPoint.x - (m_pivot.x * size.x);
			m_lastFrameRect.right = m_lastFrameRect.left + size.x;
			m_lastFrameRect.top = pivotPoint.y - (m_pivot.y * size.y);
			m_lastFrameRect.bottom = m_lastFrameRect.top + size.y;
			return m_lastFrameRect;
		}

		inline bool HitTest(D2D1_POINT_2F position)
		{
			return
				position.x >= m_lastFrameRect.left &&
				position.x <= m_lastFrameRect.right &&
				position.y >= m_lastFrameRect.top &&
				position.y <= m_lastFrameRect.bottom;
		
		}

	private:
		D2D1_RECT_F m_lastFrameRect; //maybe rename
	};

	struct UIPanel : public UIObject
	{
		ObjectType GetType() { return ObjectType::Panel; }
		D2D1_COLOR_F m_colour;
	};
}