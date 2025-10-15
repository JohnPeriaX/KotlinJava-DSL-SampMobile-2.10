#pragma once

class ListWidget : public ListBox
{
public:
	ListWidget();

	virtual void performLayout() override;

	void assemble(const std::string& data);

private:
	class ItemWidget : public ListBoxItem
	{
	public:
		struct ItemData
		{
			Label* label;
			float pos_x;
		};

	public:
		ItemWidget();

		void add(Label* label, float pos_x);

		void performLayout() override;
		virtual void draw(ImGuiRenderer* renderer) override;

	private:
		std::vector<ItemData> m_itemsData;
	};

private:
	ImVec2 m_itemSize;
};