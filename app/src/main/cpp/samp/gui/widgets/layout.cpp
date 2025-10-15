#include "../gui.h"
#include "../../log.h"

Layout::Layout(Orientation orientation)
{
	m_orientation = orientation;
	m_paddingLeft = m_paddingRight = m_paddingTop = m_paddingBottom = UISettings::padding();
	m_spacing = UISettings::padding();
}

void Layout::performLayout()
{
	if (children().empty()) return;

	int itemCount = children().size();
	ImVec2 itemSize = size() - ImVec2(m_paddingLeft + m_paddingRight, m_paddingTop + m_paddingBottom);

	// рассчитываем размеры элементов, для которых не указан размер
	for (auto c : children())
	{
		if (m_orientation == Orientation::HORIZONTAL)
		{
			if (c->width() != 0.0f)
			{
				itemSize.x -= c->width();
				itemCount--;
			}
		}
		else if (m_orientation == Orientation::VERTICAL)
		{
			if (c->height() != 0.0f)
			{
				itemSize.y -= c->height();
				itemCount--;
			}
		}
	}

	// рассчитываем размер элементов с учетом spacing
	if (m_orientation == Orientation::HORIZONTAL)
	{
		itemSize.x -= m_spacing * (children().size() - 1);
		itemSize.x /= itemCount;
	}
	else if (m_orientation == Orientation::VERTICAL)
	{
		itemSize.y -= m_spacing * (children().size() - 1);
		itemSize.y /= itemCount;
	}

	// формируем виджет
	ImVec2 curPos = ImVec2(m_paddingLeft, m_paddingTop);

	for (auto c : children())
	{
		c->setFixedSize(ImVec2(
			c->width() == 0.0f ? itemSize.x : c->width(),
			c->height() == 0.0f ? itemSize.y : c->height())
		);
		c->performLayout();
		c->setPosition(curPos);

		if (m_orientation == Orientation::HORIZONTAL)
		{
			curPos.x += m_spacing + c->width();
		}
		else if (m_orientation == Orientation::VERTICAL)
		{
			curPos.y += m_spacing + c->height();
		}
	}

	/*
	ImVec2 itemSize = ImVec2(width() - (m_paddingLeft + m_paddingRight), height() - (m_paddingTop + m_paddingBottom));

	if (m_orientation == Orientation::HORIZONTAL)
	{
		itemSize.x -= m_spacing * (itemCount - 1);
		itemSize.x /= itemCount;
	}
	else if (m_orientation == Orientation::VERTICAL)
	{
		itemSize.y -= m_spacing * (itemCount - 1);
		itemSize.y /= itemCount;
	}

	ImVec2 curPos = ImVec2(m_paddingLeft, m_paddingTop);

	for (auto c : children())
	{
		c->setFixedSize(itemSize);
		c->performLayout();
		c->setPosition(curPos);

		if (m_orientation == Orientation::HORIZONTAL)
		{
			curPos.x += m_spacing + itemSize.x;
		}
		else if (m_orientation == Orientation::VERTICAL)
		{
			curPos.y += m_spacing + itemSize.y;
		}
	}

	*/
}