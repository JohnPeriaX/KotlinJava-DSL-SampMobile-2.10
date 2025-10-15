#include "../../gui.h"

MsgBoxWidget::MsgBoxWidget()
{
	m_label = new Label("", ImColor(1.0f, 1.0f, 1.0f), false, UISettings::fontSize() / 2);
	this->addChild(m_label);
}