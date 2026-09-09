#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

struct RenderContext
{
	RenderContext() : pITextFormat(NULL),
		m_renderTarget(NULL),
		m_brush(NULL),
		Initialised(false) {
	}

	bool Initialised;
	IDWriteTextFormat* pITextFormat;
	ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1SolidColorBrush* m_brush;
};