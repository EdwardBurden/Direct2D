#include "RWindow.h"
#include <iostream>
#include "WindowsX.h"

//float DPIScale::scale = 1.0f;

RWindow::RWindow(HINSTANCE& hInstance) //dont allow any other parameters here, assign per class
{
	//ShowCursor(FALSE);
	Alive = true;
	m_factory = nullptr;
	m_renderTarget = nullptr;
	m_brushSlow = nullptr;
	m_brushFast = nullptr;
	m_mousePoint = D2D1::Point2F();
	m_ellipseSlow = D2D1::Ellipse(m_mousePoint, 20, 20);
	m_ellipseFast = m_ellipseSlow;
	m_className = L"Sample Window Class";
	m_windowClass = { };
	m_windowClass.lpfnWndProc = RWindow::WindowsMessageProcedure;
	m_windowClass.hInstance = hInstance;
	m_windowClass.lpszClassName = m_className;
	RegisterClass(&m_windowClass);
}

void RWindow::Create(HINSTANCE& hInstance, int nCmdShow) //can pass in options here
{
	HWND handle = CreateWindowEx(
		0,                              // Optional window styles.
		m_className,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		this        // Additional application data
	);
	if (handle == NULL)
		return;
	//InitializeDPIScale(m_windowHandle);
	ShowWindow(m_windowHandle, nCmdShow);
}

LRESULT CALLBACK RWindow::WindowsMessageProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RWindow* window = nullptr;
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* createptr = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = reinterpret_cast<RWindow*>(createptr->lpCreateParams);
		window->m_windowHandle = hwnd;// required becuase the line where m_windowHandle is assigned has not been run yet.
		auto ptr = reinterpret_cast<LONG_PTR>(window);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr);
	}
	else
	{
		auto ptrr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		window = (RWindow*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	if (window != nullptr)
	{
		return window->HandleMessage(uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT RWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
		if (FAILED(hr))
			return -1;
		DPIScale::Initialize(m_windowHandle);
		return 0;
	}
	case WM_DESTROY:
	{
		SafeRelease(&m_factory);
		SafeRelease(&m_renderTarget);
		SafeRelease(&m_brushSlow);
		SafeRelease(&m_brushFast);
		PostQuitMessage(0);
		Alive = false;
		return 0;
	}
	case WM_PAINT:
	{
		OnPaint();
		return 0;
	}
	case WM_SIZE:
	{
		if (m_renderTarget == NULL)
			break;

		RECT rect;
		GetClientRect(m_windowHandle, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		m_renderTarget->Resize(size);
		InvalidateRect(m_windowHandle, NULL, FALSE);
		return 0;
	}
	case WM_LBUTTONDOWN:
		SetCapture(m_windowHandle);
		InvalidateRect(m_windowHandle, NULL, FALSE);
	case WM_MOUSEMOVE:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		m_mousePoint = DPIScale::PixelsToDips(x, y);
		m_ellipseSlow.point = m_mousePoint;
		//InvalidateRect(m_windowHandle, NULL, FALSE);
		return 0;
	}
	default:
	{
		return DefWindowProc(m_windowHandle, uMsg, wParam, lParam);
	}
	}
	return TRUE;
}

HRESULT RWindow::CreateGraphicsResources()
{
	if (m_renderTarget != NULL)
		return S_OK;

	HRESULT hr = NULL;
	RECT rect;
	GetClientRect(m_windowHandle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom); // width and height from top left 
	hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(m_windowHandle, size),
		&m_renderTarget
	);
	if (FAILED(hr))
		return S_FALSE;

	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 0), &m_brushSlow);
	if (FAILED(hr))
		return S_FALSE;

	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_brushFast);
	if (FAILED(hr))
		return S_FALSE;
	//CalculateLayout

	return S_OK;
}

void RWindow::OnPaint() //render per frame
{
	HRESULT hr = CreateGraphicsResources();
	if (FAILED(hr))
		return;

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(m_windowHandle, &point);
	m_ellipseFast.point = D2D1::Point2F(point.x, point.y);

	PAINTSTRUCT paint;
	BeginPaint(m_windowHandle, &paint);
	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));

	///
	m_renderTarget->FillEllipse(m_ellipseSlow, m_brushSlow);
	m_renderTarget->FillEllipse(m_ellipseFast, m_brushFast);
	hr = m_renderTarget->EndDraw();
	if (FAILED(hr))
	{
		SafeRelease(&m_renderTarget);
		SafeRelease(&m_brushSlow);
		SafeRelease(&m_brushFast);
	}
	EndPaint(m_windowHandle, &paint);
}

