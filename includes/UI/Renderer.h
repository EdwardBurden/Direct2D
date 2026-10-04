#pragma once
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include "UI/Node.h"
#include "UI/RenderContext.h"

namespace UI
{
	class Renderer
	{
	public:
		BOOL Create(HWND handle);
		void BeginRender(HWND handle);
		void RenderNode(Node* node);
		void EndRender();
		void Resize(HWND handle);
	private:
		HRESULT CreateGraphicsResources(HWND handle);

		void DrawNode(Node* node);
		RenderContext m_context;
	};
}