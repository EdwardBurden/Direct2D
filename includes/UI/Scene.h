#pragma once
#include "UI/Node.h"
#include <queue>

namespace UI
{
	class Scene
	{
	public:
		Scene() :m_root(NULL)
		{
		}

		void RecalculateTree(RECT window);
		void Resize(HWND handle);
		UI::Node* SearchTreeForCollision(D2D1_POINT_2F point);
		UI::Node* SearchNodeForCollision(UI::Node* node, D2D1_POINT_2F point);
		UI::Node* m_root;
	};
}
