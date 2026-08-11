#include "Renderer.h"

BOOL Renderer::Create(HWND handle)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (FAILED(hr))
		return -1;
	return 0;
}

HRESULT Renderer::CreateGraphicsResources(HWND handle)
{
	if (m_renderTarget != NULL)
		return S_OK;

	HRESULT hr = NULL;
	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom); // width and height from top left 
	hr = m_factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(handle, size),
		&m_renderTarget
	);
	if (FAILED(hr))
		return S_FALSE;

	hr = m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 0), &m_brush);
	if (FAILED(hr))
		return S_FALSE;

	if (FAILED(hr))
		return S_FALSE;
	return S_OK;
}

void Renderer::BeginRender(HWND handle)
{
	HRESULT hr = CreateGraphicsResources(handle);
	if (FAILED(hr))
		return;

	m_renderTarget->BeginDraw();
	m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));
}

void Renderer::RenderUIObject(UI::UIObject* uiObject)
{
	std::queue<UI::UIObject*> queue;
	queue.push(uiObject);

	while (!queue.empty())
	{
		UI::UIObject* next = queue.front();
		queue.pop();
		DrawObject(next);
		for (auto child : next->m_children)
		{
			queue.push( child );
		}
	}
}

void Renderer::DrawObject(UI::UIObject* object)
{
	if (object->Render == NULL)
		return;

	object->Render->Draw(object->Transform, m_renderTarget, m_brush);
}

void Renderer::EndRender()
{
	HRESULT hr = m_renderTarget->EndDraw();
	if (FAILED(hr))
	{
		//clean up for next time
	}
}

void Renderer::Resize(HWND handle)
{
	if (m_renderTarget == NULL)
		return;

	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	m_renderTarget->Resize(size);
}