#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_internal.h"
#include <string>

/*
	Обертка над ImGui
*/

class ImGuiRenderer
{
public:
	ImGuiRenderer(ImDrawList* draw_list, ImFont* font);
	virtual ~ImGuiRenderer() {};

	void drawLine(const ImVec2& a, const ImVec2& b, const ImColor& color, float thickness = 1.0f);
	void drawRect(const ImVec2& a, const ImVec2& b, const ImColor& color, bool fill = false, float thickness = 1.0f);
	void drawRectFilledMulticolor(const ImVec2& a, const ImVec2& b,
		const ImColor& col_upr_left, const ImColor& col_upr_right,
		const ImColor& col_bot_right, const ImColor& col_bot_left);
	void drawTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImColor& color, bool fill = false, float thickness = 1.0f);
	void drawConvexPolyFilled(ImVec2* points, int num_points	, const ImColor& color);
	void drawText(const ImVec2& pos, const ImColor& color, const std::string& text, bool outlined = false, float font_size = 0.0f);
	void drawImage(const ImVec2& a, const ImVec2& b, ImTextureID texture);

	void pushClipRect(const ImVec2& min, const ImVec2& max, bool intersect = false);
	void popClipRect();

	ImVec2 calculateTextSize(const std::string& text, float font_size = 0.0f);

	ImColor RenderTextAndGetLastColor(const float font_size, uint8_t outline, ImVec2 pos, ImColor col, const char *szStr);
	ImVec2 CalcTextSizeWithoutTags(const float font_size, const char* szStr);

private:
	// low-level
	ImVec2 calculateTextSize(const char* begin, const char* end, float font_size = 0.0f);
	void drawText(const ImVec2& pos, const ImColor& color, const char* begin, const char* end = nullptr, bool outlined = false, float font_size = 0.0f);
	bool processInlineHexColor(const char* start, const char* end, ImVec4& color);

private:
	ImDrawList* m_drawList;
	ImFont* m_font;
};