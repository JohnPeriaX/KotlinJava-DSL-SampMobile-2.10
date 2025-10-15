#pragma once

class ListBoxItem : public Widget
{
public:
	ListBoxItem(bool selectable = true);

	void setIndex(int index) { m_index = index; }
	virtual void activateEvent(bool active);
	bool selected();

	bool selectable() { return m_selectable; }

private:
	int m_index;
	bool m_selectable;
};

class ListBox : public ScrollPanel
{
public:
	ListBox();

	void setItemSize(const ImVec2& size);
	const ImVec2& itemSize() { return m_panel->itemSize(); }

	void addItem(ListBoxItem* item);
	void removeItem(int index);

	int itemsCount() { return m_panel->itemsCount(); }
	int activeItemIndex() const { return m_panel->activeItemIndex(); }

public:
	class Panel : public Widget
	{
	public:
		Panel();

		void setItemSize(const ImVec2& size);
		const ImVec2& itemSize() { return m_itemSize; }

		void addItem(ListBoxItem* item);
		void removeItem(int index);

		int itemsCount() { return children().size(); }

		int activeItemIndex() const { return m_activeItemIndex; }
		void setActiveItemIndex(int index) { m_activeItemIndex = index; }

		virtual void performLayout() override;
		virtual void draw(ImGuiRenderer* renderer) override;

	private:
		ImVec2 m_itemSize;
		int m_activeItemIndex;
		int m_itemsCount;
	};

private:
	bool m_headed;
	Panel* m_panel;
};