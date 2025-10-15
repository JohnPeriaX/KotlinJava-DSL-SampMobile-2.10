#pragma once

class MsgBoxWidget : public ScrollPanel
{
public:
	MsgBoxWidget();

	void setMessage(const std::string& message) { m_label->setText(message); }

private:
	Label* m_label;
};