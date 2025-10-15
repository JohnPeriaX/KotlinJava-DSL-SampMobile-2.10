#pragma once

class ProgressBar : public Widget
{
public:
	ProgressBar(const ImColor& color_empty, const ImColor& color_filled, bool border = false);
	ProgressBar(const ImColor& color_empty,
		const ImColor& color_filled_upr_left, const ImColor& color_filled_upr_right,
		const ImColor& color_filled_bot_right, const ImColor& color_filled_bot_left);

	virtual void draw(ImGuiRenderer* renderer) override;

	void setValue(float value) { m_value = value; }
	float value() const { return m_value; }

private: 
	enum class ProgressBarType
	{
		DEFAULT,
		MULTICOLOR
	} m_type;

	float m_value;
	bool m_border;
	ImColor m_colEmpty;
	ImColor m_colFilled;
	ImColor m_colFilledUprLeft;
	ImColor m_colFilledUprRight;
	ImColor m_colFilledBotRight;
	ImColor m_colFilledBotLeft;
};