#pragma once

#include "msgbox.h"
#include "inputwidget.h"
#include "listwidget.h"
#include "tablistwidget.h"

enum class DialogStyle
{
	MSGBOX = 0,
	INPUT = 1,
	LIST = 2,
	PASSWORD = 3,
	TABLIST = 4,
	TABLIST_HEADERS = 5,
	NONE = -1
};

class Content : public Widget
{
public:
	Content();
	virtual void performLayout() override;

	void setActive(DialogStyle style, const std::string& data);

	const std::string& inputString() const;
	int listItem() const;

private:
	void removeActiveWidget();

private:
	Widget* m_activeWidget;
	DialogStyle m_activeWidgetStyle;
};