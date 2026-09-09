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
	UI::UIObject* SearchTree(D2D1_POINT_2F point);
	UI::UIObject* SearchTree(UI::UIObject* object, D2D1_POINT_2F point);
	UI::UIObject* m_root;
};

