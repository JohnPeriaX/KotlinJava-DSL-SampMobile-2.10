#include "../../gui.h"

InputWidget::InputWidget()
{
	m_msgbox = new MsgBoxWidget();
	this->addChild(m_msgbox);

	m_editbox = new EditBox();
	this->addChild(m_editbox);
}

void InputWidget::performLayout()
{
	float editBoxHeight = UISettings::dialogButtonPanelSize().y * 0.7f;

	/* формируем сообщение */
	m_msgbox->setMinSize(ImVec2(
		this->minSize().x,
		this->minSize().y - editBoxHeight
	));
	m_msgbox->setMaxSize(ImVec2(
		this->maxSize().x,
		this->maxSize().y - editBoxHeight
	));
	m_msgbox->performLayout();

	ImVec2 dialog_size = ImVec2(m_msgbox->width(), m_msgbox->height() + editBoxHeight);

	this->setSize(dialog_size);

	/* формируем поле ввода */
	m_editbox->setSize(ImVec2(dialog_size.x, editBoxHeight));
	m_editbox->performLayout();

	/* устанавливаем позиции  */
	m_msgbox->setPosition(ImVec2(0.0f, 0.0f));
	m_editbox->setPosition(ImVec2(0.0f, m_msgbox->position().y + m_msgbox->height()));
}

void InputWidget::setMessage(const std::string& message)
{
	m_msgbox->setMessage(message);
}

const std::string& InputWidget::inputString() const
{
	return m_editbox->text();
}