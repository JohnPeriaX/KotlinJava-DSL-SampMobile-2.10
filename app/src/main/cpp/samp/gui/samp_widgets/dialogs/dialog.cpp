#include "../../../main.h"
#include "../../gui.h"
#include "../../../game/game.h"
#include "../../../net/netgame.h"

extern CNetGame* pNetGame;

extern void MyLog(const char* fmt, ...);

extern int Button2Show;

/* Dialog */

Dialog::Dialog()
{
	/* title */
	m_title = new DialogTitle();
	this->addChild(m_title);

	m_content = new Content();
	this->addChild(m_content);

	/* button panel */
	m_buttonPanel = new DialogButtonPanel();
	this->addChild(m_buttonPanel);

	m_dialogID = -1;
}

void Dialog::show(int16_t dialog_id, DialogStyle dialog_style, const std::string& title, const std::string& message, const std::string& button1, const std::string& button2)
{
	if (dialog_id >= 0) {
		this->setTitleCaption(title);
		this->setButton1Caption(button1);
		this->setButton2Caption(button2);
		/*if (button2.length() >= 1)
		{
			Button2Show = 1;
		}
		else
		{
			Button2Show = 2;
		}
		if (Button2Show == 2) {
			this->removeChild(button2);
		}
		if (Button2Show == 1) {
			this->addChild(button2);
		}*/
		//MyLog("button2 is %i", button2.length());

		m_content->setActive(dialog_style, message);

		this->performLayout();
		this->setVisible(true);

		m_dialogID = dialog_id;
		//MyLog("Dialog ID %d", dialog_id);
	}
}

void Dialog::hide()
{
	this->setVisible(false);
}

void Dialog::setTitleCaption(const std::string& caption)
{
	m_title->setTitle(caption);
}

void Dialog::setButton1Caption(const std::string& caption)
{
	m_buttonPanel->setButton1Caption(caption);
}

void Dialog::setButton2Caption(const std::string& caption)
{
	m_buttonPanel->setButton2Caption(caption);
}

void Dialog::performLayout()
{
	/* формируем виджет контента */
	m_content->setMinSize(ImVec2(
		this->minSize().x - (UISettings::padding() + UISettings::padding()),
		this->minSize().y - (UISettings::dialogTitleHeight() + UISettings::dialogButtonPanelSize().y)
	));
	m_content->setMaxSize(ImVec2(
		this->maxSize().x - (UISettings::padding() + UISettings::padding()),
		this->maxSize().y - (UISettings::dialogTitleHeight() + UISettings::dialogButtonPanelSize().y + UISettings::padding() + UISettings::padding())
	));
	m_content->performLayout();

	/* рассчитываем размеры диалога */
	ImVec2 dialog_size = ImVec2(
		m_content->width() + (UISettings::padding() + UISettings::padding()),
		UISettings::dialogTitleHeight() + m_content->height() + UISettings::dialogButtonPanelSize().y + UISettings::padding() + UISettings::padding()
	);

	this->setSize(dialog_size);

	/* формируем заголовок */
	m_title->setSize(ImVec2(dialog_size.x, UISettings::dialogTitleHeight()));
	m_title->performLayout();

	/* формируем панель кнопок */
	m_buttonPanel->setFixedSize(UISettings::dialogButtonPanelSize());
	m_buttonPanel->performLayout();

	/* размещаем все элементы */
	m_title->setPosition(ImVec2(0.0f, 0.0f));
	m_content->setPosition(ImVec2(
		UISettings::padding(),
		m_title->position().y + m_title->height() + UISettings::padding()));
	m_buttonPanel->setPosition(ImVec2(
		(dialog_size.x - m_buttonPanel->width()) / 2,
		(dialog_size.y - m_buttonPanel->height())));

	/* позиционируем диалог по центру */
	this->setPosition(ImVec2(
		(parent()->width() - width()) / 2,
		(parent()->height() - height()) / 2));
}

void Dialog::draw(ImGuiRenderer* renderer)
{
	renderer->drawRect(absolutePosition(), absolutePosition() + size(), UISettings::dialogBackgroundColor(), true);
	Widget::draw(renderer);
}

void Dialog::touchEvent(const ImVec2& pos, TouchType type)
{
	if (m_buttonPanel->contains(pos))
	{
		m_buttonPanel->touchEvent(pos, type);
		return;
	}

	Widget::touchEvent(pos, type);
}

/* DialogTitle */
DialogTitle::DialogTitle()
{
	m_label = new Label("Title", ImColor(1.0f, 1.0f, 1.0f), false, UISettings::fontSize() / 2);
	this->addChild(m_label);
}

void DialogTitle::setTitle(const std::string& caption)
{
	m_label->setText(caption);
}

void DialogTitle::performLayout()
{
	m_label->performLayout();
	m_label->setPosition(ImVec2(UISettings::padding(), (height() - m_label->height()) / 2));
}

void DialogTitle::draw(ImGuiRenderer* renderer)
{
	renderer->drawRect(absolutePosition(), absolutePosition() + size(), UISettings::dialogTitleBackgroundColor(), true);
	Widget::draw(renderer);
}

/* DialogButtonPanel */
DialogButtonPanel::DialogButtonPanel()
	: Layout(Orientation::HORIZONTAL)
{
	m_button1 = new DialogButton1();
	this->addChild(m_button1);

	m_button2 = new DialogButton2();
	this->addChild(m_button2);
}

void DialogButtonPanel::setButton1Caption(const std::string& caption)
{
	m_button1->setCaption(caption);
}

void DialogButtonPanel::setButton2Caption(const std::string& caption)
{
	m_button2->setCaption(caption);
}

void DialogButtonPanel::performLayout()
{
	Layout::performLayout();
}

/* DialogButtonPanel::DialogButton1 */

DialogButtonPanel::DialogButton1::DialogButton1()
	: Button("", UISettings::fontSize() / 2)
{

}

void DialogButtonPanel::DialogButton1::touchPopEvent()
{
	Dialog* dialog = dynamic_cast<Dialog*>(this->parent()->parent());

	if (pNetGame) {
		pNetGame->SendDialogResponse(dialog->dialogID(), 1, dialog->content()->listItem(), dialog->content()->inputString().c_str());
	}

	dialog->hide();
}

/* DialogButtonPanel::DialogButton2 */

DialogButtonPanel::DialogButton2::DialogButton2()
	: Button("", UISettings::fontSize() / 2)
{

}

void DialogButtonPanel::DialogButton2::touchPopEvent()
{
	Dialog* dialog = dynamic_cast<Dialog*>(this->parent()->parent());

	if (pNetGame) {
		pNetGame->SendDialogResponse(dialog->dialogID(), 0, dialog->content()->listItem(), dialog->content()->inputString().c_str());
	}

	dialog->hide();
}