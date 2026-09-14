#include <string>
#include "UIRectTransform.h"
#include "RenderContext.h"

namespace UI
{
	struct UIBaseRender
	{
		virtual void Draw(const UIRectTransform& transform, RenderContext& context) = 0;
		virtual ~UIBaseRender() = default;
	};

	struct UICircleRender : public UIBaseRender
	{
		UICircleRender(D2D1::ColorF::Enum colour) :m_colour(colour), m_size(5) {}

		void Draw(const UIRectTransform& transform, RenderContext& context) override
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

	struct UITextRender : public UIBaseRender
	{
		UITextRender(const wchar_t* text) : m_text(text), m_textColour(D2D1::ColorF::Black) {}

		void Draw(const UIRectTransform& transform, RenderContext& context) override
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

	struct UISquareRender : public UIBaseRender
	{
		UISquareRender(D2D1::ColorF::Enum colour) :m_colour(colour) {}

		void Draw(const UIRectTransform& transform, RenderContext& context) override
		{
			context.Brush->SetColor(D2D1::ColorF(m_colour));
			D2D1_RECT_F rect = transform.GetRect();
			context.RenderTarget->FillRectangle(&rect, context.Brush);
		}

	private:
		D2D1::ColorF::Enum m_colour;
	};
}