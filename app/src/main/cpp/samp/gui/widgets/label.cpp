#include "..//imguirenderer.h"
#include "..//widget.h"
#include "label.h"


Label::Label(const std::string& text, const ImColor& color, bool outlined, float font_size)
	: Widget()
{
	m_text = text;
	m_color = color;
	m_outlined = outlined;
	m_fontSize = font_size;
}
void Label::setText(const std::string& text)
{
	m_text = text;
	this->performLayout();
}

void Label::performLayout()
{
	ImVec2 sz = calculateTextSize(m_text);
	this->setSize(sz);
}

void Label::draw(ImGuiRenderer* renderer)
{
	renderer->drawText(absolutePosition(), m_color, m_text, m_outlined, m_fontSize);
	Widget::draw(renderer);
}

ImVec2 Label::calculateTextSize(const std::string& text) const
{
	ImVec2 text_size = { 0.0f, 0.0f };
	if (text.empty()) return text_size;

	ImVec2 cur_size = { 0.0f, 0.0f };

	float font_size = m_fontSize == 0.0f ? ImGui::GetIO().Fonts->Fonts[0]->FontSize : m_fontSize;
	const char* text_start = text.c_str();
	const char* text_cur = text.c_str();
	const char* text_end = text.c_str() + text.length();

	while (*text_cur)
	{
		if (*text_cur == '{' && ((&text_cur[7] < text_end) && text_cur[7] == '}'))
		{
			if (text_cur != text_start)
			{
				// текст до колор-кода
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			text_cur += 7;
			text_start = text_cur + 1;
		}
		else if (*text_cur == '\n')
		{
			if (text_cur != text_start)
			{
				// текст до \n
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			// обновляем text_size
			text_size.x = ImMax(text_size.x, cur_size.x);
			cur_size.y += font_size;
			cur_size.x = 0.0f;

			text_start = text_cur + 1;
		}
		else if (*text_cur == '\t')
		{
			if (text_cur != text_start)
			{
				// текст до \t
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			cur_size.x += font_size;
			text_start = text_cur + 1;
		}

		++text_cur;
	}

	if (text_cur != text_start)
	{
		// текст без форматирования
		ImVec2 sz = calculateTextSize(text_start, text_cur);
		cur_size.x += sz.x;
		if (cur_size.y == 0.0f) cur_size.y = sz.y;
	}

	text_size = ImMax(text_size, cur_size);
	return text_size;
}

ImVec2 Label::calculateTextSize(const char* begin, const char* end) const
{
	ImFont* font = ImGui::GetIO().Fonts->Fonts[0];
	return font->CalcTextSizeA(m_fontSize == 0.0f ? font->FontSize : m_fontSize, FLT_MAX, 0.0f, begin, end);
}



//LabelImage

LabelImage::LabelImage(const std::string& text, const ImColor& color, bool outlined, float font_size)
	: Widget()
{
	//m_text = text;
	//Encoding::cp2utf(text);
	m_text = text;
	m_color = color;
	m_outlined = outlined;
	m_fontSize = font_size;
}
void LabelImage::setText(const std::string& text)
{
	m_text = text;
	this->performLayout();
}

void LabelImage::performLayout()
{
	ImVec2 sz = calculateTextSize(m_text);
	this->setSize(sz);
}

void LabelImage::draw(ImGuiRenderer* renderer)
{
	renderer->drawText(absolutePosition(), m_color, m_text, m_outlined, m_fontSize);
	Widget::draw(renderer);
}

ImVec2 LabelImage::calculateTextSize(const std::string& text) const
{
	ImVec2 text_size = { 0.0f, 0.0f };
	if (text.empty()) return text_size;

	ImVec2 cur_size = { 0.0f, 0.0f };

	float font_size = m_fontSize == 0.0f ? ImGui::GetIO().Fonts->Fonts[0]->FontSize : m_fontSize;
	const char* text_start = text.c_str();
	const char* text_cur = text.c_str();
	const char* text_end = text.c_str() + text.length();

	while (*text_cur)
	{
		if (*text_cur == '{' && ((&text_cur[7] < text_end) && text_cur[7] == '}'))
		{
			if (text_cur != text_start)
			{
				// текст до колор-кода
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			text_cur += 7;
			text_start = text_cur + 1;
		}
		else if (*text_cur == '\n')
		{
			if (text_cur != text_start)
			{
				// текст до \n
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			// обновляем text_size
			text_size.x = ImMax(text_size.x, cur_size.x);
			cur_size.y += font_size;
			cur_size.x = 0.0f;

			text_start = text_cur + 1;
		}
		else if (*text_cur == '\t')
		{
			if (text_cur != text_start)
			{
				// текст до \t
				ImVec2 sz = calculateTextSize(text_start, text_cur);
				cur_size.x += sz.x;
				if (cur_size.y == 0.0f) cur_size.y = sz.y;
			}

			cur_size.x += font_size;
			text_start = text_cur + 1;
		}

		++text_cur;
	}

	if (text_cur != text_start)
	{
		// текст без форматирования
		ImVec2 sz = calculateTextSize(text_start, text_cur);
		cur_size.x += sz.x;
		if (cur_size.y == 0.0f) cur_size.y = sz.y;
	}

	text_size = ImMax(text_size, cur_size);
	return text_size;
}

ImVec2 LabelImage::calculateTextSize(const char* begin, const char* end) const
{
	ImFont* font = ImGui::GetIO().Fonts->Fonts[0];
	return font->CalcTextSizeA(m_fontSize == 0.0f ? font->FontSize : m_fontSize, FLT_MAX, 0.0f, begin, end);
}