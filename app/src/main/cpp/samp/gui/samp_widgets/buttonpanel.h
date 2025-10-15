#pragma once

class ButtonPanel : public Layout
{
public:
	ButtonPanel();

	CButton* m_bH;
private:
	Button* m_bAlt;
	Button* m_bY;
	Button* m_bN;

};