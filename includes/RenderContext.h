#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

struct RenderContext
{
	RenderContext() : TextFormat(NULL), RenderTarget(NULL), Brush(NULL), Initialised(false) {}

	bool Initialised;
	IDWriteTextFormat* TextFormat;
	ID2D1HwndRenderTarget* RenderTarget;
	ID2D1SolidColorBrush* Brush;
};