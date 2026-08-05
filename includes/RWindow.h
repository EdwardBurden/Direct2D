#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")


class DPIScale
{
	static float scale;

public:
	static void Initialize(HWND hwnd)
	{
		float dpi = GetDpiForWindow(hwnd);
		scale = dpi / 96.0f;
	}

	template <typename T>
	static D2D1_POINT_2F PixelsToDips(T x, T y)
	{
		return D2D1::Point2F(static_cast<float>(x) / scale, static_cast<float>(y) / scale);
	}
};

class RWindow
{
public:	bool Alive;
	RWindow(HINSTANCE& hInstance);
	void Create(HINSTANCE& hInstance, int nCmdShow);
	void OnPaint();

//todo define custom constructor and move/copies

private:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK WindowsMessageProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT CreateGraphicsResources();

	template <class T> void SafeRelease(T** ppT) //todo potion into declare and define
	{
		if (*ppT)
		{
			(*ppT)->Release();
			*ppT = NULL;
		}
	}

	LPCWSTR m_className;
	WNDCLASS m_windowClass;
	HWND m_windowHandle;

	ID2D1Factory* m_factory;
	ID2D1HwndRenderTarget* m_renderTarget;
	ID2D1SolidColorBrush* m_brushSlow;
	ID2D1SolidColorBrush* m_brushFast;

	D2D1_ELLIPSE m_ellipseSlow;
	D2D1_ELLIPSE m_ellipseFast;

	D2D1_POINT_2F m_mousePoint;
};