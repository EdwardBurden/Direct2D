#include "Renderer.h"

BOOL Renderer::Create(HWND handle)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if (FAILED(hr))
		return -1;

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,__uuidof(IDWriteFactory),reinterpret_cast<IUnknown**>(&pIDWriteFactory));
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
		&m_context.m_renderTarget
	);
	hr = pIDWriteFactory->CreateTextFormat(
		L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		10.0f * 96.0f / 72.0f,
		L"en-US",
		&m_context.pITextFormat
	);

	//m_context.pITextFormat.style


	if (FAILED(hr))
		return S_FALSE;

	hr = m_context.m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(1, 1, 0), &m_context.m_brush);
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

	m_context.m_renderTarget->BeginDraw();
	m_context.m_renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));
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

	object->Render->Draw(object->Transform, m_context);
}

void Renderer::EndRender()
{
	HRESULT hr = m_context.m_renderTarget->EndDraw();
	if (FAILED(hr))
	{
		//clean up for next time
	}
}

void Renderer::Resize(HWND handle)
{
	if (m_context.m_renderTarget == NULL)
		return;

	RECT rect;
	GetClientRect(handle, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right, rect.bottom);
	m_context.m_renderTarget->Resize(size);
}