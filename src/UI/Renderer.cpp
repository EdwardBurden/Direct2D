#include "UI/Renderer.h"
#include <queue>

namespace UI {
	BOOL Renderer::Create(HWND handle)
	{
		HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
		if (FAILED(hr))
			return -1;

		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pIDWriteFactory));
		if (FAILED(hr))
			return -1;


		return 0;
	}

	HRESULT Renderer::CreateGraphicsResources(HWND handle) //conext
	{
		if (m_context.Initialised)
			return S_OK;

		HRESULT hr = NULL;
		RECT rect;
		GetClientRect(handle, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom); // width and height from top left 
		hr = m_factory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(handle, size),
			&m_context.RenderTarget
		);
		hr = pIDWriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			10.0f * 96.0f / 72.0f,
			L"en-US",
			&m_context.TextFormat
		);

		//m_context.pITextFormat.style


		if (FAILED(hr))
			return S_FALSE;

		hr = m_context.RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 0), &m_context.Brush);
		if (FAILED(hr))
			return S_FALSE;

		if (FAILED(hr))
			return S_FALSE;

		m_context.Initialised = true;
		return S_OK;
	}

	void Renderer::BeginRender(HWND handle)
	{
		HRESULT hr = CreateGraphicsResources(handle);
		if (FAILED(hr))
			return;

		m_context.RenderTarget->BeginDraw();
		m_context.RenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));
	}

	void Renderer::RenderNode(Node* uiObject)
	{
		std::queue<Node*> queue;
		queue.push(uiObject);

		while (!queue.empty())
		{
			Node* next = queue.front();
			queue.pop();
			DrawNode(next);
			for (auto child : next->Children)
			{
				queue.push(child);
			}
		}
	}

	void Renderer::DrawNode(Node* object)
	{
		if (object->Graphic == NULL)
			return;

		object->Graphic->Draw(object->Transform, m_context);
	}

	void Renderer::EndRender()
	{
		HRESULT hr = m_context.RenderTarget->EndDraw();
		if (FAILED(hr))
		{
			//clean up for next time
		}
	}

	void Renderer::Resize(HWND handle)
	{
		if (m_context.RenderTarget == NULL)
			return;

		RECT rect;
		GetClientRect(handle, &rect);
		D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
		m_context.RenderTarget->Resize(size);
	}
}