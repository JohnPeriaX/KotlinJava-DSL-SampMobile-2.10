#pragma once

#include <functional>

class Button : public Widget
{
public:
	Button(const std::string& caption, float font_size = 0.0f);

	const std::string& caption() const { return m_label->text(); }
	void setCaption(const std::string& caption) { m_label->setText(caption); }

	std::function<void()> callback() { return m_callback; }
	void setCallback(const std::function<void()>& callback) { m_callback = callback; }
	
	void setColor(const ImColor& color) { m_color = color; }
	void setColorFocused(const ImColor& color) { m_colorFocused = color; }
	void setCaptionColor(const ImColor& color) { m_label->setColor(color); }

	virtual void performLayout() override;

	virtual void draw(ImGuiRenderer* renderer) override;

	virtual void touchPopEvent() override;

private:
	Label* m_label;
	std::function<void()> m_callback;

	ImColor m_color;
	ImColor m_colorFocused;
};

class CButton : public Widget
{
public:
	CButton(const std::string& caption, float font_size = 0.0f);

	const std::string& caption() const { return m_label->text(); }
	void setCaption(const std::string& caption) { m_label->setText(caption); }

	std::function<void()> callback() { return m_callback; }
	void setCallback(const std::function<void()>& callback) { m_callback = callback; }

	void setColor(const ImColor& color) { m_color = color; }
	void setColorFocused(const ImColor& color) { m_colorFocused = color; }

	virtual void performLayout() override;

	virtual void draw(ImGuiRenderer* renderer) override;

	virtual void touchPopEvent() override;

private:
	Label* m_label;
	std::function<void()> m_callback;

	ImColor m_color;
	ImColor m_colorFocused;
};

//>> Button
class OButton : public Widget
{
public:
	OButton(const std::string& caption, float font_size = 0.0f);

	const std::string& caption() const { return m_label->text(); }
	void setCaption(const std::string& caption) { m_label->setText(caption); }

	std::function<void()> callback() { return m_callback; }
	void setCallback(const std::function<void()>& callback) { m_callback = callback; }

	void setColor(const ImColor& color) { m_color = color; }
	void setColorFocused(const ImColor& color) { m_colorFocused = color; }

	virtual void performLayout() override;

	virtual void draw(ImGuiRenderer* renderer) override;

	virtual void touchPopEvent() override;

private:
	Label* m_label;
	std::function<void()> m_callback;

	ImColor m_color;
	ImColor m_colorFocused;
};