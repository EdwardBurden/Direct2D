#pragma once
#include <vector>
#include <functional>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

namespace UI
{

	enum ObjectType
	{
		Panel,
		Circle
	};


	struct UIRectTransform
	{
	public:
		UIRectTransform() {}
		inline D2D1_RECT_F GetRect() const { return m_rectPosition; }
		inline void CalculateRect(D2D1_RECT_F parent)
		{
			D2D1_POINT_2F parentSize = { parent.right - parent.left , parent.bottom - parent.top };

			D2D1_RECT_F area;
			area.left = parent.left + (Anchors.left * parentSize.x);
			area.right = parent.left + (Anchors.right * parentSize.x);
			area.top = parent.top + (Anchors.top * parentSize.y);
			area.bottom = parent.top + (Anchors.bottom * parentSize.y);

			D2D1_POINT_2F size;
			size.x = (area.right - area.left) + SizeDelta.x;
			size.y = (area.bottom - area.top) + SizeDelta.y;

			D2D1_POINT_2F pivotPoint;
			pivotPoint.x = (area.left + ((area.right - area.left) * Pivot.x)) + AnchoredOffset.x;
			pivotPoint.y = (area.top + ((area.bottom - area.top) * Pivot.y)) + AnchoredOffset.y;

			m_rectPosition.left = pivotPoint.x - (Pivot.x * size.x);
			m_rectPosition.right = m_rectPosition.left + size.x;
			m_rectPosition.top = pivotPoint.y - (Pivot.y * size.y);
			m_rectPosition.bottom = m_rectPosition.top + size.y;
		}
		D2D1_RECT_F Anchors;
		D2D1_POINT_2F Pivot;
		D2D1_POINT_2F AnchoredOffset;
		D2D1_POINT_2F SizeDelta;
	private:
		D2D1_RECT_F m_rectPosition;
	};

	struct UIBaseRender
	{
		inline virtual void Draw(UIRectTransform& transform, ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* m_brush) = 0; //todo put brushes in render contet class container an dpass in
		//draw method
	};

	struct UICircleRender : public UIBaseRender
	{
		//draw method
	};

	struct UISquareRender : public UIBaseRender
	{
		inline void Draw(UIRectTransform& transform, ID2D1HwndRenderTarget* renderTarget, ID2D1SolidColorBrush* m_brush) override
		{
			m_brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetRect();
			renderTarget->FillRectangle(&rect, m_brush);
		}
		D2D1::ColorF::Enum m_colour;
		//draw method
	};

	struct UIInteractable
	{
		//todo have a way to detect base on setup, circle, rectangle image etc. So detection can match render component but doesnt need to know about it.
		inline bool HitTest(const UIRectTransform& transform, D2D1_POINT_2F test)
		{
			auto rect = transform.GetRect();
			return
				test.x >= rect.left &&
				test.x <= rect.right &&
				test.y >= rect.top &&
				test.y <= rect.bottom; //simple version for now

		}
		std::function<void(int)> OnClick;


		// on click event, on hover event etc
	};

	struct UIObject //not abstract, keep simple with one concrete for now
	{
		const char* DebugName;
		// keep the components like this as nullable componentns for now to keep things easy to clean up later
		UIRectTransform Transform;	//never null
		UIBaseRender* Render;//nullable
		UIInteractable* m_interaction;
		std::vector<UIObject*> m_children;



	};

	inline static UIObject* CreateExampleButton()
	{
		UIObject* base = new UIObject();
		base->Render = new UISquareRender();
		base->m_interaction = new UIInteractable();
		base->m_interaction->OnClick = [base](int x)
			{
				auto thing = static_cast<UISquareRender*>(base->Render);
				thing->m_colour = D2D1::ColorF::Aqua;
			}; //example

	}


	/*struct UIObject
	{
		D2D1_RECT_F m_calculatedRect;

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

	struct Slider : public UIObject
	{
		//Slider, with text and rag ability     <---0--> 4.5/10
		// should ahve setup for setting min and max, and staring value
		// needs a panel to cover totoal area,
		// needs a child object to render the line
		// needs a child object to rende the circle
		// cirlce needs to be draggable inside the area of the parent and limited on y axis
		// on circle draggin the slider needs to be updated and told the numebr to draw
		// should be able to get the slider value

	};
	*/
}