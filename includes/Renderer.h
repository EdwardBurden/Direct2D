#pragma once
#include "Scene.h"
#include "UIObject.h"

class Renderer
{
public:
	Renderer() :
		m_factory(NULL),
		m_renderTarget(NULL),
		m_brush(NULL)
	{}
	BOOL Create(HWND handle);
	void BeginRender(HWND handle);
	void RenderUIObject(UI::UIObject* uiObject);
	void EndRender();
	void Resize(HWND handle);
private:
	HRESULT CreateGraphicsResources(HWND handle);

	void DrawObject(UI::UIObject* object);
	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1SolidColorBrush* m_brush;
};
