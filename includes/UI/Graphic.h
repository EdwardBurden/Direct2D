#pragma once
#include <string>
#include "UI/RectTransform.h"
#include "UI/RenderContext.h"

namespace UI
{
	class Graphic
	{
	public:
		virtual void Draw(const RectTransform& transform, RenderContext& context) = 0;
		virtual void TransformUpdated() {}
		virtual ~Graphic() = default;
	};

	class CircleGraphic : public Graphic
	{
	public:
		CircleGraphic(D2D1::ColorF::Enum colour, size_t size) :m_colour(colour), m_size(size) {}

		void Draw(const RectTransform& transform, RenderContext& context) override
		{
			context.Brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetRect();
			D2D1_ELLIPSE ellipse = D2D1::Ellipse({ rect.left + ((rect.right - rect.left) / 2), rect.top + ((rect.bottom - rect.top) / 2) }, m_size, m_size);
			context.RenderTarget->FillEllipse(&ellipse, context.Brush);
		}
	private:
		D2D1::ColorF::Enum m_colour;
		int m_size;
	};

	class TextGraphic : public Graphic
	{
	public:
		TextGraphic(const wchar_t* text) : m_layout(NULL), m_text(text), m_textColour(D2D1::ColorF::Black) , m_autoSize(true), m_fontSize(15)
		{
		
		}

		void Draw(const RectTransform& transform, RenderContext& context) override
		{
			if (m_layout == NULL)
			{
				HRESULT hr = context.pIDWriteFactory->CreateTextLayout(
					m_text.c_str(),
					static_cast<UINT32>(m_text.length()),
					context.TextFormat,
					1000,
					1000,
					&m_layout
					);
			}
			D2D1_RECT_F	 rect = transform.GetRect();
			m_layout->SetMaxHeight(rect.bottom - rect.top);
			m_layout->SetMaxWidth(rect.right - rect.left);
			context.Brush->SetColor(D2D1::ColorF(m_textColour));
			DWRITE_TEXT_RANGE range{ 0,static_cast<UINT32>(m_text.length()) };
	


			m_layout->SetFontSize(m_fontSize, range);
			D2D1_POINT_2F point = { transform.GetRect().left,transform.GetRect().top };
	

			context.RenderTarget->DrawTextLayout(
				point,
				m_layout,
				context.Brush,
				D2D1_DRAW_TEXT_OPTIONS_CLIP
			);
		}

		void TransformUpdated() override 
		{


		}

	private:
		D2D1::ColorF::Enum m_textColour;
		std::wstring m_text;
		float m_fontSize;
		bool m_autoSize;
		IDWriteTextLayout* m_layout;
	};

	class SquareGraphic : public Graphic
	{
	public:
		SquareGraphic(D2D1::ColorF::Enum colour) :m_colour(colour) {}

		void Draw(const RectTransform& transform, RenderContext& context) override
		{
			context.Brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetRect();
			context.RenderTarget->FillRectangle(&rect, context.Brush);
		}

	private:
		D2D1::ColorF::Enum m_colour;
	};
}