#pragma once
#include "BaseWindow.h"
#include <functional>

namespace Window
{
	class MainWindow : public BaseWindow<MainWindow> 
	{
		friend class BaseWindow<MainWindow>;
	public:
		MainWindow();
		void PollMessages();
		std::function<void(POINT p)> ClickEvent; 
		std::function<void(POINT p)> DraggingEvent; 
		std::function<void(POINT p)> UnClickEvent; 
		std::function<void(RECT rect)> WindowResize;
		std::function<void()> WindowClosed;
		void BeginWindowPaint();
		void EndWindowPaint();

		BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0) override;
		void RenderWindow();
	protected:
		PCWSTR GetClassString() const { return L"Main"; }
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void OnLeftMouseButtonDown_Message(int pixelX, int pixelY, DWORD flags);
		void OnLeftMouseButtonUp_Message(int pixelX, int pixelY, DWORD flags);
		void OnMouseMove_Message(int pixelX, int pixelY, DWORD flags);
	private:
		PAINTSTRUCT m_paint;
	};
}