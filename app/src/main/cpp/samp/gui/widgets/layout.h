#pragma once

enum class Orientation
{
	HORIZONTAL,
	VERTICAL
};

class Layout : public Widget
{
public:
	Layout(Orientation orientation = Orientation::HORIZONTAL);

	void setPadding(float padding) { m_paddingLeft = m_paddingRight = m_paddingTop = m_paddingBottom = padding; }
	void setPaddingLeft(float padding) { m_paddingLeft = padding; }
	void setPaddingRight(float padding) { m_paddingRight = padding; }
	void setPaddingTop(float padding) { m_paddingTop = padding; }
	void setPaddingBottom(float padding) { m_paddingBottom = padding; }

	float paddingLeft() const { return m_paddingLeft; }
	float paddingRight() const { return m_paddingRight; }
	float paddingTop() const { return m_paddingTop; }
	float paddingBottom() const { return m_paddingBottom; }

	void setSpacing(float spacing) { m_spacing = spacing; }
	float spacing() const { return m_spacing; }

	void setOrientation(Orientation orientation) { m_orientation = orientation; }
	Orientation orientation() const { return m_orientation; }

	void performLayout() override;

private:
	Orientation m_orientation;
	float m_paddingLeft;
	float m_paddingRight;
	float m_paddingTop;
	float m_paddingBottom;
	float m_spacing;
};