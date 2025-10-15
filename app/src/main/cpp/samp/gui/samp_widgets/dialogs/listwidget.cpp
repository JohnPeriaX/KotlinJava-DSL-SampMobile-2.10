#include "../../../main.h"
#include "../../gui.h"

#include <sstream>

/* ListWidget */

ListWidget::ListWidget()
	: ListBox()
{
	m_itemSize = { 0.0f, UISettings::dialogListItemHeight() };
}

void ListWidget::performLayout()
{
	m_itemSize.x = ImMax(m_itemSize.x, this->minSize().x);
	this->setItemSize(m_itemSize);
	ListBox::performLayout();
}

void ListWidget::assemble(const std::string& data)
{
	std::vector < std::vector<Label*> > vRows;

	/* Читаем строки */
	std::stringstream ss_data(data);
	std::string s_row;
	while (std::getline(ss_data, s_row, '\n'))
	{
		if (s_row.length() == 0) continue;

		std::vector<Label*> vRowItems;

		/* Читаем элементы строк */
		std::stringstream ss_row(s_row);
		std::string s_item;
		while (std::getline(ss_row, s_item, '\t'))
		{
			Label* label = new Label(s_item, ImColor(1.0f, 1.0f, 1.0f), false, UISettings::fontSize() / 2);
			label->performLayout();
			vRowItems.push_back(label);
		}

		vRows.push_back(vRowItems);
	}

	/* Рассчитываем оптимальную ширину для каждого столбца */
	std::vector<float> vColumnsWidth;
	for (int r_idx = 0; r_idx < vRows.size(); r_idx++)
	{
		for (int c_idx = 0; c_idx < vRows[r_idx].size(); c_idx++)
		{
			if (vColumnsWidth.size() < (c_idx + 1)) {
				vColumnsWidth.push_back(0.0f);
			}

			vColumnsWidth[c_idx] = std::max(vColumnsWidth[c_idx], vRows[r_idx][c_idx]->width());
		}
	}

	float max_width = 0.0f;
	for (int r_idx = 0; r_idx < vRows.size(); r_idx++)
	{
		ItemWidget* item = new ItemWidget();
		this->addItem(item);

		float pos_x = UISettings::padding();
		for (int c_idx = 0; c_idx < vRows[r_idx].size(); c_idx++)
		{
			Label* l = vRows[r_idx][c_idx];
			item->add(l, pos_x);
			pos_x += vColumnsWidth[c_idx] + UISettings::padding() * 5;
			max_width = std::max(max_width, pos_x);
		}
	}

	m_itemSize.x = max_width;
}

/* ListWidget::ItemWidget */
ListWidget::ItemWidget::ItemWidget()
{

}

void ListWidget::ItemWidget::performLayout()
{
	for (auto item : m_itemsData)
	{
		Label* l = item.label;
		l->setPosition(ImVec2(
			item.pos_x,
			(height() - l->height()) / 2)
		);
	}
}

void ListWidget::ItemWidget::draw(ImGuiRenderer* renderer)
{
	if (this->selected()) renderer->drawRect(absolutePosition(), absolutePosition() + size(), ImColor(0xF5, 0x91, 0x32)/*ImColor(0.7f, 0.1f, 0.1f)*/, true);
	ListBoxItem::draw(renderer);
}

void ListWidget::ItemWidget::add(Label* label, float pos_x)
{
	ItemData data;
	data.label = label;
	data.pos_x = pos_x;
	m_itemsData.push_back(data);

	this->addChild(label);
}