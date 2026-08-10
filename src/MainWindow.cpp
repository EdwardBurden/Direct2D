#include "MainWindow.h"
#include "WindowsUtils.h"
#include "WindowsX.h"


MainWindow::MainWindow() : m_scene(NULL), m_mousePoint(D2D1::Point2F())
{
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

void MainWindow::RenderWindow()
{
	PAINTSTRUCT paint;
	BeginPaint(m_windowHandle, &paint);
	m_scene->DrawScene(m_windowHandle, m_mousePoint);
	EndPaint(m_windowHandle, &paint);
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		BeginPaint(m_windowHandle, &paint);
		m_scene->DrawScene(m_windowHandle, m_mousePoint);
		EndPaint(m_windowHandle, &paint);
		return 0;
	}
	case WM_SIZE:
	{
		m_scene->Resize(m_windowHandle);
		InvalidateRect(m_windowHandle, NULL, FALSE);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		OnLeftMouseButtonDown_Message(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		OnLeftMouseButtonUp_Message(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove_Message(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);
		return 0;
	}
	default:
		return DefWindowProc(m_windowHandle, uMsg, wParam, lParam);
	}
}

void MainWindow::OnLeftMouseButtonDown_Message(int pixelX, int pixelY, DWORD flags)
{
	SetCapture(m_windowHandle);
	m_mousePoint = DPIScale::PixelsToDips(pixelX, pixelY); //todo test if this makes a difference on different scales
	m_scene->SetMouseState(1);
	m_scene->OnMouseClick(m_windowHandle, m_mousePoint);
	InvalidateRect(m_windowHandle, NULL, FALSE);

}

void MainWindow::OnLeftMouseButtonUp_Message(int pixelX, int pixelY, DWORD flags)
{
	m_mousePoint = DPIScale::PixelsToDips(pixelX, pixelY);
	m_scene->SetMouseState(2);
	ReleaseCapture();
}

void MainWindow::OnMouseMove_Message(int pixelX, int pixelY, DWORD flags)
{
	m_mousePoint = DPIScale::PixelsToDips(pixelX, pixelY);
	if (flags & MK_LBUTTON)  //if flags contians left down
	{
		m_scene->SetMouseState(3);
	}
	else
	{
		m_scene->SetMouseState(0);
	}
}
