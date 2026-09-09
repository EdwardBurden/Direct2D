#pragma once
#include <vector>
#include <functional>
#include "RenderContext.h"

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
		inline D2D1_RECT_F GetWorldRect() const { return m_rectPosition; }
		inline void CalculateRect(D2D1_RECT_F parent)
		{
			m_parent = parent;
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

		inline D2D1_POINT_2F WorldToAnchoredOffset(D2D1_POINT_2F worldPoint)
		{
			D2D1_POINT_2F parentSize = { m_parent.right - m_parent.left , m_parent.bottom - m_parent.top };

			float anchorMinX = m_parent.left + Anchors.left * parentSize.x;
			float anchorMinY = m_parent.top + Anchors.top * parentSize.y;
			float anchorMaxX = m_parent.left + Anchors.right * parentSize.x;
			float anchorMaxY = m_parent.top + Anchors.bottom * parentSize.y;

			float refX = anchorMinX + (anchorMaxX - anchorMinX) * Pivot.x;
			float refY = anchorMinY + (anchorMaxY - anchorMinY) * Pivot.y;

			return D2D1_POINT_2F{ worldPoint.x - refX, worldPoint.y - refY };
		}


		D2D1_RECT_F Anchors;
		D2D1_POINT_2F Pivot;
		D2D1_POINT_2F AnchoredOffset;
		D2D1_POINT_2F SizeDelta;
		D2D1_RECT_F m_parent;

		//D2D1_RECT_F m_temp;

	private:
		D2D1_RECT_F m_rectPosition;
	};

	struct UIBaseRender
	{
		inline virtual void Draw(UIRectTransform& transform, const RenderContext& context) = 0; //todo put brushes in render contet class container an dpass in
		//draw method
	};

	struct UICircleRender : public UIBaseRender
	{
		UICircleRender(D2D1::ColorF::Enum color) :m_colour(color) {}
		inline void Draw(UIRectTransform& transform, const RenderContext& context) override
		{
			context.m_brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetWorldRect();
			D2D1_ELLIPSE ellipse = D2D1::Ellipse({ rect.left + ((rect.right - rect.left) / 2), rect.top + ((rect.bottom - rect.top) / 2) }, 5, 5);
			context.m_renderTarget->FillEllipse(&ellipse, context.m_brush);
		}
		D2D1::ColorF::Enum m_colour;
	};

	struct UITextRender : public UIBaseRender
	{
		UITextRender(const char* title) : m_title(title) {}
		const char* m_title;
		inline void Draw(UIRectTransform& transform, const RenderContext& context) override
		{
			context.m_renderTarget->DrawText(
				L"Hello World",
				wcslen(L"Hello World"),
				context.pITextFormat,
				transform.GetWorldRect(),
				context.m_brush
			);
		}
	};

	struct UISquareRender : public UIBaseRender
	{
		UISquareRender(D2D1::ColorF::Enum color) :m_colour(color) {}

		inline void Draw(UIRectTransform& transform, const RenderContext& context) override
		{
			context.m_brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetWorldRect();
			context.m_renderTarget->FillRectangle(&rect, context.m_brush);
		}
		D2D1::ColorF::Enum m_colour;
		//draw method
	};

	struct UIInteractable
	{
		//todo have a way to detect base on setup, circle, rectangle image etc. So detection can match render component but doesnt need to know about it.
		inline bool HitTest(const UIRectTransform& transform, D2D1_POINT_2F test)
		{
			auto rect = transform.GetWorldRect();
			return
				test.x >= rect.left &&
				test.x <= rect.right &&
				test.y >= rect.top &&
				test.y <= rect.bottom; //simple version for now

		}
		std::function<void(POINT p)> OnPointerDown;
		std::function<void(POINT p)> OnPointerUp;
		std::function<void(POINT p)> OnPointerDrag;

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

	inline static UIObject* Createbackground()
	{
		UIObject* base = new UIObject();
		base->Transform.Anchors = { 0,0,1,1 };
		base->Transform.Pivot = { 0.5f,0.5f };
		base->Transform.SizeDelta = { 0,0 };
		base->Transform.AnchoredOffset = { 0,0 };
		return base;
	}


	struct Slider : public UIObject
	{
		//drag on the whole area listener, then move circle to poss, it doesnt listen
			//Slider, with text and rag ability     <---0--> 4.5/10
			// should ahve setup for setting min and max, and staring value
			// needs a panel to cover totoal area,
			// needs a child object to render the line
			// needs a child object to rende the circle
			// cirlce needs to be draggable inside the area of the parent and limited on y axis
			// on circle draggin the slider needs to be updated and told the numebr to draw
			// should be able to get the slider value

	};

	inline static UIObject* CreateDraggableExample(const char* title, D2D1_RECT_F anchors = { 0,0,1,1 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.0f,0.0f })
	{
		Slider* base = new Slider();
		base->Transform.Anchors = anchors;
		base->Transform.Pivot = pivot;
		base->Transform.SizeDelta = size;
		base->Transform.AnchoredOffset = offset;
		base->Render = new UISquareRender(D2D1::ColorF::Red);
		base->m_interaction = new UIInteractable();

		UIObject* ball = new UIObject();
		ball->Transform.Anchors = { 0.5f,0.5f,0.5f,0.5f };
		ball->Transform.Pivot = { 0.5f,0.5f };
		ball->Transform.SizeDelta = { 20,20 };
		ball->Transform.AnchoredOffset = { 0,0 };
		ball->Render = new UICircleRender(D2D1::ColorF::Blue);
		base->m_children.push_back(ball);

		base->m_interaction->OnPointerDrag = [base, ball](POINT p)
			{
				ball->Transform.AnchoredOffset = ball->Transform.WorldToAnchoredOffset(D2D1::Point2F(p.x, p.y));
			};
		return base;
	}

	inline static UIObject* CreateExampleButton(const char* title, std::function<void(UI::UIObject*)> buttonClick, D2D1_RECT_F anchors = { 0.5,0.5,0.5,0.5 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.5f,0.5f })
	{
		UIObject* base = new UIObject();
		base->Transform.Anchors = anchors;
		base->Transform.Pivot = pivot;
		base->Transform.SizeDelta = size;
		base->Transform.AnchoredOffset = offset;
		base->Render = new UISquareRender(D2D1::ColorF::Black);
		base->m_interaction = new UIInteractable();
		base->m_interaction->OnPointerDown = [base, buttonClick](POINT p)
			{
				buttonClick(base);
			};

		UIObject* text = new UIObject();
		text->Transform.Anchors = { 0,0,1,1 };
		text->Transform.Pivot = { 0.5f,0.5f };
		text->Transform.SizeDelta = { -10,-10 };
		text->Transform.AnchoredOffset = { 0,0 };
		text->Render = new UITextRender(title);
		base->m_children.push_back(text);
		return base;
	}
}