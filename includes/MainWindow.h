#include "BaseWindow.h"
#include "Scene.h"


class MainWindow : public BaseWindow<MainWindow> //todo move to file
{
	friend class BaseWindow<MainWindow>;
public:
	MainWindow();
	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0) override;
	void SetScene(Scene* scene) { m_scene = scene; };
	void RenderWindow();
protected:
	PCWSTR GetClassString() const { return L"Main"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void OnLeftMouseButtonDown_Message(int pixelX, int pixelY, DWORD flags);
	void OnLeftMouseButtonUp_Message(int pixelX, int pixelY, DWORD flags);
	void OnMouseMove_Message(int pixelX, int pixelY, DWORD flags);
private:
	Scene* m_scene;
	D2D1_POINT_2F m_mousePoint;
};