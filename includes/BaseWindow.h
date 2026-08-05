#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

template<class T>
class BaseWindow
{
public:
	BaseWindow();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0);
	HWND GetWindowHandle() const { return m_windowHandle; }
private:
protected:
	WNDCLASS m_windowClass;
	HWND m_windowHandle;
	virtual PCWSTR GetClassString() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

template<class T>
inline BaseWindow<T>::BaseWindow() : m_windowClass(NULL), m_windowHandle(NULL)
{
}

template<class T>
inline LRESULT BaseWindow<T>::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	T* window = nullptr;
	if (uMsg == WM_NCCREATE)
	{
		CREATESTRUCT* createptr = reinterpret_cast<CREATESTRUCT*>(lParam);
		window = reinterpret_cast<T*>(createptr->lpCreateParams);
		window->m_windowHandle = hwnd;// required becuase the line where m_windowHandle is assigned has not been run yet.
		auto ptr = reinterpret_cast<LONG_PTR>(window);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, ptr);
	}
	else
	{
		window = (T*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}
	if (window != nullptr)
	{
		return window->HandleMessage(uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

template<class T>
inline BOOL BaseWindow<T>::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu)
{
	m_windowClass = { };
	m_windowClass.lpfnWndProc = T::WindowProc;
	m_windowClass.hInstance = GetModuleHandle(NULL);
	m_windowClass.lpszClassName = GetClassString();
	RegisterClass(&m_windowClass);

	HWND handle = CreateWindowEx(
		dwExStyle,
		GetClassString(),
		lpWindowName, 
		dwStyle, 
		x, 
		y,
		nWidth, 
		nHeight,
		hWndParent,
		hMenu, 
		GetModuleHandle(NULL), 
		this
	);
	return handle ? TRUE : FALSE;
}