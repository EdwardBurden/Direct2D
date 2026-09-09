#pragma once
#include "Scene.h"
#include "RenderContext.h"

class Renderer
{
public:
	BOOL Create(HWND handle);
	void BeginRender(HWND handle);
	void RenderUIObject(UI::UIObject* uiObject);
	void EndRender();
	void Resize(HWND handle);
private:
	HRESULT CreateGraphicsResources(HWND handle);

	void DrawObject(UI::UIObject* object);
	ID2D1Factory* m_factory;
	IDWriteFactory* pIDWriteFactory = NULL;

	RenderContext m_context;
};
