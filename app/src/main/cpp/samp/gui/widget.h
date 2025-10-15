#pragma once

#include <vector>
#include <string>

enum class TouchType
{
	push,
	move,
	pop,
	none
};

class Widget
{
public:
	Widget();

	/* parent */
	Widget* parent() const { return m_parent; }
	void setParent(Widget* parent) { m_parent = parent; }

	/* visible */
	bool visible() { return m_visible; }
	void setVisible(bool visible) { m_visible = visible; }

	/* size */
	const ImVec2& size() const { return m_size; }
	void setSize(const ImVec2& size);
	float width() const { return m_size.x; }
	void setWidth(float width);
	float height() const { return m_size.y; }
	void setHeight(float height);

	/* min/max size */
	const ImVec2& maxSize() const { return m_maxSize; }
	void setMaxSize(const ImVec2& max_size) { m_maxSize = max_size; }
	const ImVec2& minSize() const { return m_minSize; }
	void setMinSize(const ImVec2& min_size) { m_minSize = min_size; }
	void setFixedSize(const ImVec2& fixed_size) { m_size = m_minSize = m_maxSize = fixed_size; }

	/* position */
	const ImVec2& position() const { return m_position; }
	void setPosition(const ImVec2& position);
	const ImVec2& absolutePosition() const { return m_absolutePosition; }

	/* children */
	int childCount() { return m_children.size(); }
	const std::vector<Widget*>& children() { return m_children; }
	void addChild(int index, Widget* widget);
	void addChild(Widget* widget);
	void removeChild(int index);
	void removeChild(const Widget* widget);
	Widget* childAt(int index) const { return m_children[index]; }
	int childIndex(Widget* widget) const;

	bool focused() { return m_focused; }
	bool active() { return m_active; }

	/* touch */
	bool contains(const ImVec2& pos) const;
	virtual void touchEvent(const ImVec2& pos, TouchType type);

	/* draw/perform */
	virtual void draw(ImGuiRenderer* renderer);
	virtual void performLayout();

	/* нажатие на виджет */
	virtual void touchPushEvent() {}
	/* изменение позиции курсора (при условии что он был нажат) */
	virtual void touchMoveEvent(const ImVec2& delta) {}
	/* отжатие виджета (при условии что он был нажат) */
	virtual void touchPopEvent() {}

	/* виджет становится активным */
	virtual void activateEvent(bool active) {}
	/* фокус на виджете */
	virtual void focuseEvent(bool event) {}
	/* введен текст */
	virtual void keyboardEvent(const std::string& input) {}

protected:
	virtual ~Widget();

private:
	void updateAbsolutePosition();

	void setActive(bool active);
	void setFocus(bool focus);

private:
	Widget* m_parent;
	bool m_visible;
	ImVec2 m_position;
	ImVec2 m_size;
	ImVec2 m_maxSize;
	ImVec2 m_minSize;

	ImVec2 m_absolutePosition;

	bool m_focused;
	bool m_active;
	ImVec2 m_pushAbsolutePos;
	ImVec2 m_moveDelta;

	std::vector<Widget*> m_children;
};