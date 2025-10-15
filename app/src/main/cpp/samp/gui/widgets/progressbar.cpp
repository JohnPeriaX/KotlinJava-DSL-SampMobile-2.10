#include "../gui.h"

ProgressBar::ProgressBar(const ImColor& color_empty, const ImColor& color_filled, bool border)
{
	m_type = ProgressBarType::DEFAULT;

	m_colEmpty = color_empty;
	m_colFilled = color_filled;
	m_border = border;
}

ProgressBar::ProgressBar(const ImColor& color_empty,
	const ImColor& color_filled_upr_left, const ImColor& color_filled_upr_right,
	const ImColor& color_filled_bot_right, const ImColor& color_filled_bot_left)
{
	m_type = ProgressBarType::MULTICOLOR;

	m_colEmpty = color_empty;
	m_colFilledUprLeft = color_filled_upr_left;
	m_colFilledUprRight = color_filled_upr_right;
	m_colFilledBotRight = color_filled_bot_right;
	m_colFilledBotLeft = color_filled_bot_left;
}

void ProgressBar::draw(ImGuiRenderer* renderer)
{
	// background
	renderer->drawRect(absolutePosition(), absolutePosition() + size(), m_colEmpty, true);

	// scissor test
	renderer->pushClipRect(absolutePosition(), absolutePosition() + ImVec2(width() * value(), height()));

	// fill
	switch (m_type)
	{
	case ProgressBarType::DEFAULT:
		renderer->drawRect(absolutePosition(), absolutePosition() + size(), m_colFilled, true);
		break;

	case ProgressBarType::MULTICOLOR:
		renderer->drawRectFilledMulticolor(absolutePosition(), absolutePosition() + size(),
			m_colFilledUprLeft, m_colFilledUprRight, m_colFilledBotRight, m_colFilledBotLeft);
		break;
	}

	renderer->popClipRect();

	// border
	if (m_border)
		renderer->drawRect(absolutePosition(), absolutePosition() + size(), ImColor(0.0f, 0.0f, 0.0f), false, UISettings::outlineSize());
}