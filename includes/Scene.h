#pragma once
#include "UIObject.h"
#include <queue>
/*
Renderer/scene for the objects beeing renderered
holds the tree of ui objects
*/

class Scene
{
public:
	Scene() :m_root(NULL)
	{
	}

	void RecalculateTree(RECT window);
	void Resize(HWND handle);
	static Scene* CreateExampleScene();
	UI::UIObject* m_root;
};

