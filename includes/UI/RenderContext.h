#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

namespace UI
{
	struct RenderContext
	{
		RenderContext() : m_factory(NULL), pIDWriteFactory(NULL), TextFormat(NULL), RenderTarget(NULL), Brush(NULL), Initialised(false) {}

		bool Initialised;
		ID2D1Factory* m_factory;
		IDWriteFactory* pIDWriteFactory;
		IDWriteTextFormat* TextFormat;
		ID2D1HwndRenderTarget* RenderTarget;
		ID2D1SolidColorBrush* Brush;
	};
}