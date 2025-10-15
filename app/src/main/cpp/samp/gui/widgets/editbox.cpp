#include "../gui.h"

extern UI* pUI;

EditBox::EditBox()
{
	m_label = new Label(" ", ImColor(1.0f, 1.0f, 1.0f), false, UISettings::fontSize() / 2);//...
	this->addChild(m_label);
}

void EditBox::performLayout()
{
	m_label->performLayout();

	m_label->setPosition(ImVec2(
		UISettings::padding(),

		(height() - m_label->height()) / 2
	));
}

void EditBox::draw(ImGuiRenderer* renderer)
{
	renderer->drawRect(
		absolutePosition() + ImVec2(UISettings::outlineSize(), UISettings::outlineSize()), 
		(absolutePosition() + size()) - ImVec2(UISettings::outlineSize(), UISettings::outlineSize()), 
		ImColor(0.287f, 0.287f, 0.287f, 0.890f)/*ImColor(0xF5, 0x91, 0x32)*/, false, UISettings::outlineSize());//

	Widget::draw(renderer);
}

void EditBox::touchPopEvent()
{
	pUI->keyboard()->show(this);
}

void EditBox::keyboardEvent(const std::string& input)
{
	m_input = input;
	m_label->setText(Encoding::cp2utf(input));
}