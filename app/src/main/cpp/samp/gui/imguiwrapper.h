#pragma once

#include "imguirenderer.h"
#include "../game/RW/RenderWare.h"
#include <vector>

/*
	Wrapper class for ImGui
*/

class ImGuiWrapper
{
public:
	ImGuiWrapper(const ImVec2& display_size, const std::string& font_path);
    ImGuiWrapper(const ImVec2& display_size, const std::vector<char>& fontData);

	virtual bool initialize();

	virtual void render();

	virtual void shutdown();

	ImGuiRenderer* renderer() const { return m_renderer; }
	
	void renderDrawData(ImDrawData* draw_data);

protected:
	virtual ~ImGuiWrapper();

	virtual void drawList() = 0;

	const ImVec2& displaySize() { return m_displaySize; }

private:
	void setupRenderState(ImDrawData* draw_data);
	
	void checkVertexBuffer(ImDrawData* draw_data);
	
	bool createFontTexture();
	
	void destroyFontTexture();

private:
	ImVec2 m_displaySize;
	std::string m_fontPath;
    std::vector<char> m_fontData;

	ImGuiRenderer* m_renderer;
	RwRaster* m_fontRaster;

	RwIm2DVertex* m_vertexBuffer;
	int m_vertexBufferSize;
};
