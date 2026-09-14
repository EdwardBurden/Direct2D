#include <d2d1.h>

namespace UI
{
	struct UIRectTransform
	{
	public:
		UIRectTransform( ) : Anchors({ 0.5,0.5,0.5,0.5 }), Pivot({ 0.5,0.5 }), AnchoredOffset({ 0,0 }), SizeDelta({ 0,0 }), m_rectPosition({ 0,0,0,0 }) {}
		D2D1_RECT_F GetRect() const { return m_rectPosition; }
		void CalculateRect(D2D1_RECT_F parent)
		{
			D2D1_POINT_2F parentSize = GetRectSize(parent);
			D2D1_RECT_F area;
			area.left = parent.left + (Anchors.left * parentSize.x);
			area.right = parent.left + (Anchors.right * parentSize.x);
			area.top = parent.top + (Anchors.top * parentSize.y);
			area.bottom = parent.top + (Anchors.bottom * parentSize.y);

			D2D1_POINT_2F size;
			size.x = (area.right - area.left) + SizeDelta.x;
			size.y = (area.bottom - area.top) + SizeDelta.y;

			D2D1_POINT_2F pivotPoint;
			pivotPoint.x = (area.left + ((area.right - area.left) * Pivot.x)) + AnchoredOffset.x;
			pivotPoint.y = (area.top + ((area.bottom - area.top) * Pivot.y)) + AnchoredOffset.y;

			m_rectPosition.left = pivotPoint.x - (Pivot.x * size.x);
			m_rectPosition.right = m_rectPosition.left + size.x;
			m_rectPosition.top = pivotPoint.y - (Pivot.y * size.y);
			m_rectPosition.bottom = m_rectPosition.top + size.y;
		}

		D2D1_POINT_2F WorldToAnchoredOffset(D2D1_POINT_2F worldPoint, D2D1_RECT_F parent) const
		{
			D2D1_POINT_2F parentSize = GetRectSize(parent);
			float anchorMinX = parent.left + Anchors.left * parentSize.x;
			float anchorMinY = parent.top + Anchors.top * parentSize.y;
			float anchorMaxX = parent.left + Anchors.right * parentSize.x;
			float anchorMaxY = parent.top + Anchors.bottom * parentSize.y;

			float refX = anchorMinX + (anchorMaxX - anchorMinX) * Pivot.x;
			float refY = anchorMinY + (anchorMaxY - anchorMinY) * Pivot.y;

			return D2D1_POINT_2F{ worldPoint.x - refX, worldPoint.y - refY };
		}

		static D2D1_POINT_2F GetRectSize(D2D1_RECT_F parent)
		{
			return { parent.right - parent.left , parent.bottom - parent.top };
		}

		D2D1_RECT_F Anchors;
		D2D1_POINT_2F Pivot;
		D2D1_POINT_2F AnchoredOffset;
		D2D1_POINT_2F SizeDelta;
	private:
		D2D1_RECT_F m_rectPosition;
	};
}