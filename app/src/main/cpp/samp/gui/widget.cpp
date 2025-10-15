#include "gui.h"
#include<algorithm>

Widget::Widget()
{
	m_parent = nullptr;
	m_visible = true;
	m_position = ImVec2(0.0f, 0.0f);
	m_size = ImVec2(0.0f, 0.0f);

	m_minSize = { 0.0f, 0.0f };
	m_maxSize = { FLT_MAX, FLT_MAX };

	m_focused = false;
	m_active = false;
	m_pushAbsolutePos = { 0.0f, 0.0f };
	m_moveDelta = { 0.0f, 0.0f };
}

Widget::~Widget()
{
	for (auto child : m_children)
	{
		if (child) delete child;
	}
}

void Widget::setSize(const ImVec2& size)
{
	ImVec2 sz = size;

	/* min */
	if (sz.x < m_minSize.x) sz.x = m_minSize.x;
	if (sz.y < m_minSize.y) sz.y = m_minSize.y;
	/* max */
	if (sz.x > m_maxSize.x) sz.x = m_maxSize.x;
	if (sz.y > m_maxSize.y) sz.y = m_maxSize.y;

	m_size = sz;
}

void Widget::setWidth(float width)
{
	/* min */
	if (width < m_minSize.x) width = m_minSize.x;
	/* max */
	if (width > m_maxSize.x) width = m_maxSize.x;

	m_size.x = width;
}

void Widget::setHeight(float height)
{
	/* min */
	if (height < m_minSize.y) height = m_maxSize.y;
	/* max */
	if (height > m_maxSize.y) height = m_maxSize.y;

	m_size.y = height;
}

void Widget::setPosition(const ImVec2& position)
{
	// relative-pos
	m_position = position;

	// пересчитываем абсолютную позицию для this и child
	this->updateAbsolutePosition();
}

void Widget::updateAbsolutePosition()
{
	m_absolutePosition = m_parent ? m_parent->absolutePosition() + m_position : m_position;

	if (m_children.empty()) return;
	for (auto child : m_children)
	{
		child->updateAbsolutePosition();
	}
}

void Widget::setFocus(bool focus)
{
	m_focused = focus;
	focuseEvent(focus);
}

void Widget::setActive(bool active)
{
	m_active = active;
	activateEvent(active);
}

void Widget::addChild(int index, Widget* widget)
{
	assert(index <= childCount());
	m_children.insert(m_children.begin() + index, widget);
	widget->setParent(this);
}

void Widget::addChild(Widget* widget)
{
	addChild(childCount(), widget);
}

void Widget::removeChild(const Widget* widget)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), widget), m_children.end());
	delete widget;
}

void Widget::removeChild(int index)
{
	Widget* widget = m_children[index];
	m_children.erase(m_children.begin() + index);
	delete widget;
}

int Widget::childIndex(Widget* widget) const
{
	auto it = std::find(m_children.begin(), m_children.end(), widget);
	if (it == m_children.end()) return -1;
	return (int)(it - m_children.begin());
}

bool Widget::contains(const ImVec2& pos) const
{
	ImVec2 d = pos - absolutePosition();
	return (
		d.x >= 0 && d.y >= 0 &&
		d.x < size().x && d.y < size().y);
}

void Widget::touchEvent(const ImVec2& pos, TouchType type)
{
	if (type == TouchType::push)
	{
		if (visible() && contains(pos))
		{
			setFocus(true);
			m_pushAbsolutePos = pos;
			touchPushEvent();
		}
		else
		{
			setActive(false);
		}
	}
	else if (type == TouchType::move)
	{
		ImVec2 delta = pos - m_pushAbsolutePos;
		if (m_focused) touchMoveEvent(delta - m_moveDelta);
		m_moveDelta = delta;
	}
	else if (type == TouchType::pop)
	{
		bool focused = m_focused;
		setFocus(false);

		if (visible() && contains(pos) && focused) 
		{
			setActive(true);
			touchPopEvent();
		}

		m_pushAbsolutePos = { 0.0f, 0.0f };
		m_moveDelta = { 0.0f, 0.0f };
	}

	/* проходим по дочерним окнам */
	if (m_children.empty()) return;
	for (auto child : m_children)
	{
		if(child->visible()) child->touchEvent(pos, type);
	}
}

void Widget::draw(ImGuiRenderer* renderer)
{

	if (!m_children.empty())
	{
		for (auto child : m_children)
		{
			if (child->visible())
			{
#if DEBUG_GUI == 0
				//renderer->pushClipRect(child->absolutePosition(), child->absolutePosition() + child->size(), true);
#endif
				child->draw(renderer);
#if DEBUG_GUI == 0
				//renderer->popClipRect();
#endif
			}
		}
	}

#if DEBUG_GUI == 1
	renderer->drawRect(absolutePosition(), absolutePosition() + size(), m_active ? ImColor(0, 255, 0) : m_focused ? ImColor(0, 0, 255) : ImColor(255, 0, 0), false);
#endif
}

void Widget::performLayout()
{
	for (auto c : m_children)
	{
		c->performLayout();
	}
} 