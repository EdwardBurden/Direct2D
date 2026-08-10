#pragma once
#include "UIObject.h"
/*
Renderer/scene for the objects beeing renderered
holds the tree of ui objects
*/

class Scene
{
public:
	Scene() :
		m_factory(NULL),
		m_renderTarget(NULL),
		m_brush(NULL),
		m_debugMouse(D2D1::Ellipse(D2D1::Point2F(), 5, 5)),
		m_debugMouseColour(D2D1::ColorF::Black) ,
		m_selected(NULL)
	{
	}

	UI::UIObject* m_root;
	HRESULT CreateScene();
	void DrawScene(HWND handle, D2D1_POINT_2F mousePoint);
	void DrawObject(UI::UIObject* object, D2D1_RECT_F parent);
	void Resize(HWND handle);
	void DrawSceneTree(UI::UIObject* object, D2D1_RECT_F parent);
	void OnMouseClick(HWND handle, D2D1_POINT_2F mousePoint);
	//0,non:1,down,2:up.3:drag
	void SetMouseState(int state);
private:
	HRESULT CreateGraphicsResources(HWND handle);
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1SolidColorBrush* m_brush;

	D2D1_ELLIPSE m_debugMouse;
	D2D1::ColorF::Enum m_debugMouseColour;
	UI::UIObject* m_selected;
};

