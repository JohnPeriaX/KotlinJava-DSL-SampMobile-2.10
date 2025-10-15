#pragma once

#include "content.h"

class DialogButtonPanel : public Layout
{
public:
	DialogButtonPanel();

	void setButton1Caption(const std::string& caption);
	void setButton2Caption(const std::string& caption);

	virtual void performLayout() override;

private:
	class DialogButton1 : public Button
	{
	public:
		DialogButton1();

		virtual void touchPopEvent() override;
	};

	class DialogButton2 : public Button
	{
	public:
		DialogButton2();

		virtual void touchPopEvent() override;
	};

	DialogButton1* m_button1;
	DialogButton2* m_button2;
};

class DialogTitle : public Widget
{
public:
	DialogTitle();

	void setTitle(const std::string& caption);

	virtual void performLayout() override;
	virtual void draw(ImGuiRenderer* renderer) override;

private:
	Label* m_label;
};

class Dialog : public Widget
{
public:
	Dialog();

	void show(int16_t wDialogID, DialogStyle dialog_style, const std::string& title, const std::string& message, const std::string& button1, const std::string& button2);
	void hide();

	virtual void performLayout() override;
	virtual void draw(ImGuiRenderer* renderer) override;

	virtual void touchEvent(const ImVec2& pos, TouchType type) override;

	Content* content() const { return m_content; }
	int16_t dialogID() const { return m_dialogID; }

private:
	void setTitleCaption(const std::string& caption);
	void setButton1Caption(const std::string& caption);
	void setButton2Caption(const std::string& caption);

private:
	DialogTitle* m_title;
	Content* m_content;
	DialogButtonPanel* m_buttonPanel;

	int16_t m_dialogID;
};