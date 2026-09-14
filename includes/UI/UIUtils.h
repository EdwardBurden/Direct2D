#include "UIObject.h"

namespace UI
{
	static UIObject* CreateBackground()
	{
		UIObject* base = new UIObject(L"Background", NULL);
		base->Transform.Anchors = { 0,0,1,1 };
		base->Transform.Pivot = { 0,0 };
		base->Render = new UISquareRender(D2D1::ColorF::Black);
		return base;
	}

	static UIObject* CreatePanel(UIObject* parent, const wchar_t* name, D2D1::ColorF::Enum color, D2D1_RECT_F anchors = { 0,0,1,1 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.0f,0.0f })
	{
		UIObject* panel = new UIObject(name, parent);
		panel->Transform.Anchors = anchors;
		panel->Transform.Pivot = pivot;
		panel->Transform.SizeDelta = size;
		panel->Transform.AnchoredOffset = offset;
		panel->Render = new UISquareRender(color);
		panel->Collider = new UIInteractionCollider();
		return panel;
	}

	static UIObject* CreateHierarchyPanel(UIObject* parent, const wchar_t* name, D2D1::ColorF::Enum color)
	{
		UIObject* panel = new UIObject(name, parent);
		panel->Transform.Anchors = { 0,0,1,0 };
		panel->Transform.Pivot = { 1,0 };
		panel->Transform.SizeDelta = { 0,20 };
		panel->Transform.AnchoredOffset = { 0,0 };
		panel->Render = new UISquareRender(color);
		panel->Collider = new UIInteractionCollider();
		UIObject* text = new UIObject(L"text", panel);
		text->Transform.Anchors = { 0,0,1,1 };
		text->Transform.Pivot = { 0,0 };
		text->Transform.SizeDelta = { 0,0 };
		text->Transform.AnchoredOffset = { 0,0 };
		text->Render = new UITextRender(name);
		return panel;
	}
}