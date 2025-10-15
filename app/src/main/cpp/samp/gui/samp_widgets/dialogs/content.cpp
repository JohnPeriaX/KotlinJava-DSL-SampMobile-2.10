#include "../../gui.h"

Content::Content()
{
    m_activeWidgetStyle = DialogStyle::NONE;
    m_activeWidget = nullptr;
}

void Content::performLayout()
{
    if (m_activeWidget == nullptr) return;

    m_activeWidget->setMinSize(this->minSize());
    m_activeWidget->setMaxSize(this->maxSize());
    m_activeWidget->performLayout();
    m_activeWidget->setPosition(ImVec2(0.0f, 0.0f));

    this->setSize(m_activeWidget->size());
}

void Content::setActive(DialogStyle style, const std::string& data)
{
    removeActiveWidget();

    switch (style)
    {
    case DialogStyle::MSGBOX:
    {
        MsgBoxWidget* msgbox = new MsgBoxWidget();
        this->addChild(msgbox);
        msgbox->setMessage(data);
        m_activeWidget = msgbox;
        break;
    }
    case DialogStyle::PASSWORD:
    case DialogStyle::INPUT:
    {
        InputWidget* input = new InputWidget();
        this->addChild(input);
        input->setMessage(data);
        m_activeWidget = input;
        break;
    }

    case DialogStyle::TABLIST:
    case DialogStyle::LIST:
    {
        ListWidget* list = new ListWidget();
        this->addChild(list);
        list->assemble(data);
        m_activeWidget = list;
        break;
    }

    case DialogStyle::TABLIST_HEADERS:
    {
        TabListWidget* tablist = new TabListWidget();
        this->addChild(tablist);
        tablist->assemble(data);
        m_activeWidget = tablist;
        break;
    }
	}

    m_activeWidgetStyle = style;
    m_activeWidget->setVisible(true);
}

void Content::removeActiveWidget()
{
    if (m_activeWidget != nullptr) {
        this->removeChild(m_activeWidget);
    }

    m_activeWidget = nullptr;
    m_activeWidgetStyle = DialogStyle::NONE;
}

const std::string& Content::inputString() const
{
    if (m_activeWidgetStyle == DialogStyle::INPUT ||
        m_activeWidgetStyle == DialogStyle::PASSWORD) {
        return dynamic_cast<InputWidget*>(m_activeWidget)->inputString();
    }

    return "";
}

int Content::listItem() const
{
    if (m_activeWidgetStyle == DialogStyle::LIST ||
        m_activeWidgetStyle == DialogStyle::TABLIST || 
        m_activeWidgetStyle == DialogStyle::TABLIST_HEADERS) {
        return dynamic_cast<ListBox*>(m_activeWidget)->activeItemIndex();
    }

    return -1;
}