#include "UI/Scene.h"
#include <queue>

namespace UI
{
	void Scene::RecalculateTree(RECT window)
	{
		// Create Queue of nodes to process along with their parentRect.
		std::queue<std::pair<UI::Node*, D2D1_RECT_F>> layers;
		D2D1_RECT_F parent = { window.left, window.top, window.right, window.bottom };
		//Place the Root and window rect as the first node to process.
		layers.push({ m_root,  parent });

		// Each node in the tree will be processed by depth layer in the tree first.
		while (!layers.empty())
		{
			auto& [node, parentRect] = layers.front();
			layers.pop();

			// Calculate the parent node, then place the calculated rect on the queue along with each child node.
			D2D1_RECT_F result = node->Transform.CalculateRect(parentRect);
			for (auto child : node->Children)
			{
				layers.push({ child, result });
			}
		}
	}


	UI::Node* Scene::SearchTreeForCollision(D2D1_POINT_2F point)
	{
		return SearchNodeForCollision(m_root, point);
	}

	UI::Node* Scene::SearchNodeForCollision(UI::Node* node, D2D1_POINT_2F point)
	{
		if (node == nullptr)
			return nullptr;

		// Get reverse order of the way children are drawn.
		for (std::vector<UI::Node*>::reverse_iterator it = node->Children.rbegin(); it != node->Children.rend(); ++it)
		{
			// Recursively search for colider passed hit test.
			UI::Node* nodeHit = SearchNodeForCollision(*it, point);
			if (nodeHit != NULL)
				return nodeHit;
		}

		// Check point is in the colliders area.
		if (node->Collider != nullptr && node->Collider->HitTest(node->Transform, point))
			return node;

		return nullptr;
	}

	void Scene::Resize(HWND handle)
	{

	}
}