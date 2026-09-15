#pragma once
#include <string>
#include "UI/RectTransform.h"
#include "UI/RenderContext.h"

namespace UI
{
	struct Graphic
	{
		virtual void Draw(const RectTransform& transform, RenderContext& context) = 0;
		virtual ~Graphic() = default;
	};

	struct CircleGraphic : public Graphic
	{
		CircleGraphic(D2D1::ColorF::Enum colour) :m_colour(colour), m_size(5) {}

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

	struct TextGraphic : public Graphic
	{
		TextGraphic(const wchar_t* text) : m_text(text), m_textColour(D2D1::ColorF::Black) {}

		void Draw(const RectTransform& transform, RenderContext& context) override
		{
			context.Brush->SetColor(D2D1::ColorF(m_textColour));
			context.RenderTarget->DrawText(
				m_text.c_str(),
				static_cast<UINT32>(m_text.length()),
				context.TextFormat,
				transform.GetRect(),
				context.Brush
			);
		}

	private:
		D2D1::ColorF::Enum m_textColour;
		std::wstring m_text;
	};

	struct SquareGraphic : public Graphic
	{
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