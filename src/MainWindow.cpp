#include "MainWindow.h"
#include "WindowsUtils.h"
#include "WindowsX.h"


MainWindow::MainWindow() : /*m_scene(NULL),*/ m_mousePoint(D2D1::Point2F())
{
}

void MainWindow::PollMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void MainWindow::BeginWindowPaint()
{
	BeginPaint(m_windowHandle, &m_paint);
}

void MainWindow::EndWindowPaint()
{
	EndPaint(m_windowHandle, &m_paint);
}

BOOL MainWindow::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
	if (BaseWindow<MainWindow>::Create(lpWindowName, dwStyle, dwExStyle, x, y, nWidth, nHeight, hWndParent, hMenu))
	{
		DPIScale::Initialize(m_windowHandle);
		return TRUE;
	}
	return FALSE;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		ValidateRect(m_windowHandle, nullptr); // just clears the dirty flag, no drawing
		return 0;
	}
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(m_windowHandle, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		if (WindowResize) {
			WindowResize(size); //could send just handle to remove all d2d usage
		}
		InvalidateRect(m_windowHandle, NULL, FALSE);
		return 0;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
	{
		if (MouseInput) 
		{
			MouseInput(uMsg, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		}
		return 0;
	}
	default:
		return DefWindowProc(m_windowHandle, uMsg, wParam, lParam);
	}
}
