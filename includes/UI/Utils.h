#pragma once
#include "UI/Node.h"

namespace UI
{
	static Node* CreateBackground()
	{
		Node* base = new Node(L"Background", NULL);
		base->Transform.Anchors = { 0,0,1,1 };
		base->Transform.Pivot = { 0,0 };
		base->Graphic = new SquareGraphic(D2D1::ColorF::Black);
		return base;
	}

	static Node* CreatePanel(Node* parent, const wchar_t* name, D2D1::ColorF::Enum color, D2D1_RECT_F anchors = { 0,0,1,1 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.0f,0.0f })
	{
		Node* panel = new Node(name, parent);
		panel->Transform.Anchors = anchors;
		panel->Transform.Pivot = pivot;
		panel->Transform.SizeDelta = size;
		panel->Transform.AnchoredOffset = offset;
		panel->Graphic = new SquareGraphic(color);
		panel->Collider = new Collider();
		return panel;
	}

	static Node* CreateButton(Node* parent, const wchar_t* name, D2D1::ColorF::Enum color, D2D1_RECT_F anchors = { 0,0,1,1 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.0f,0.0f })
	{
		Node* panel = new Node(name, parent);
		panel->Transform.Anchors = anchors;
		panel->Transform.Pivot = pivot;
		panel->Transform.SizeDelta = size;
		panel->Transform.AnchoredOffset = offset;
		panel->Graphic = new CircleGraphic(color, 20);
		panel->Collider = new Collider();
		return panel;
	}

	static Node* CreateText(Node* parent, const wchar_t* name, const wchar_t* text, D2D1_RECT_F anchors = { 0,0,1,1 }, D2D1_POINT_2F size = { 0,0 }, D2D1_POINT_2F offset = { 0,0 }, D2D1_POINT_2F pivot = { 0.5f,0.5f })
	{
		Node* panel = new Node(name, parent);
		panel->Transform.Anchors = anchors;
		panel->Transform.Pivot = pivot;
		panel->Transform.SizeDelta = size;
		panel->Transform.AnchoredOffset = offset;
		panel->Graphic = new TextGraphic(text);
		panel->Collider = new Collider();
		return panel;
	}

	static Node* CreateHierarchyPanel(Node* parent, const wchar_t* name, D2D1::ColorF::Enum color)
	{
		Node* panel = new Node(name, parent);
		panel->Transform.Anchors = { 0,0,1,0 };
		panel->Transform.Pivot = { 1,0 };
		panel->Transform.SizeDelta = { 0,20 };
		panel->Transform.AnchoredOffset = { 0,0 };
		panel->Graphic = new SquareGraphic(color);
		panel->Collider = new Collider();
		Node* text = new Node(L"text", panel);
		text->Transform.Anchors = { 0,0,1,1 };
		text->Transform.Pivot = { 0,0 };
		text->Transform.SizeDelta = { 0,0 };
		text->Transform.AnchoredOffset = { 0,0 };
		text->Graphic = new TextGraphic(name);
		return panel;
	}
}