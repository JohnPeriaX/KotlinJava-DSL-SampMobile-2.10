#pragma once

class ScrollPanel : public Widget
{
public:
	ScrollPanel();

	void setScrollableX(bool scrollableX) { m_scrollableX = scrollableX; }
	void setScrollableY(bool scrollableY) { m_scrollableY = scrollableY; }
	void setScrollable(bool scrollable) { m_scrollableX = m_scrollableY = scrollable; }
	void setScrollX(float x);
	void setScrollY(float y);

	virtual void performLayout() override;

	virtual void draw(ImGuiRenderer* renderer) override;

	virtual void touchMoveEvent(const ImVec2& delta) override;


private:
	ImVec2 m_scroll;
	bool m_scrollableX;
	bool m_scrollableY;
};