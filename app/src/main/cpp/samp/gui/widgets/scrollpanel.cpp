#include "../gui.h"

ScrollPanel::ScrollPanel()
{
	m_scroll = ImVec2(0.0f, 0.0f);
	m_scrollableX = true;
	m_scrollableY = true;
}


void ScrollPanel::setScrollX(float x)
{
	if (children().empty()) return;
	Widget* child = children()[0];

	m_scroll.x = x;
	ImVec2 deltaPos = ImVec2(
		-m_scroll.x * (child->width() - width()),
		-m_scroll.y * (child->height() - height())
	);

	child->setPosition(deltaPos);
}

void ScrollPanel::setScrollY(float y)
{
	if (children().empty()) return;
	Widget* child = children()[0];

	m_scroll.y = y;
	ImVec2 deltaPos = ImVec2(
		-m_scroll.x * (child->width() - width()),
		-m_scroll.y * (child->height() - height())
	);

	child->setPosition(deltaPos);
}

void ScrollPanel::performLayout()
{
	if (children().empty()) return;
	if (children().size() > 1) return;

	Widget* child = children()[0];

	child->performLayout();
	child->setPosition(ImVec2(0.0f, 0.0f));

	this->setSize(child->size());
}

void ScrollPanel::draw(ImGuiRenderer* renderer)
{
	Widget::draw(renderer);
}

void ScrollPanel::touchMoveEvent(const ImVec2& delta)
{
	if (children().empty()) return;
	Widget* child = children()[0];

	/*if (m_scrollableX && child->width() > width())
	{
		float scrollX = width() * ImMin(1.0f, width() / child->width());
		m_scroll.x = ImMax(0.0f, ImMin(1.0f, m_scroll.x - delta.x / (child->width() + scrollX)));
	}

	if (m_scrollableY && child->height() > height())
	{
		float scrollY = height() * ImMin(1.0f, height() / child->height());
		m_scroll.y = ImMax(0.0f, ImMin(1.0f, m_scroll.y - delta.y / (child->height() + scrollY)));
	}

	ImVec2 deltaPos = ImVec2(
		-m_scroll.x * (child->width() - width()),
		-m_scroll.y * (child->height() - height())
	);*/

	ImVec2 deltaPos = child->position();
	ImVec2 deltaMinPos = { width() - child->width(), height() - child->height() };

	if (m_scrollableX)
	{
		deltaPos.x += delta.x;
	}

	if (m_scrollableY)
	{
		deltaPos.y += delta.y;
	}

	if (deltaPos.x < deltaMinPos.x) deltaPos.x = deltaMinPos.x;
	if (deltaPos.y < deltaMinPos.y) deltaPos.y = deltaMinPos.y;
	if (deltaPos.x > 0) deltaPos.x = 0.0f;
	if (deltaPos.y > 0) deltaPos.y = 0.0f;
	
	child->setPosition(deltaPos);
}