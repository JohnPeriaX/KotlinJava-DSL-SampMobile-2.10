#pragma once

#include "imguirenderer.h"
#include "../game/RW/RenderWare.h"

/*
	класс-прослойка между imgui и рендер-движком
*/

class ImGuiWrapper
{
public:
	ImGuiWrapper(const ImVec2& display_size, const std::string& font_path);

	/*
		инициализирует imgui
		- создание контекста imgui
		- установка размера экрана
		- загрузка файла шрифта
	*/
	virtual bool initialize();

	/*
		рендерит построенные в drawList примитивы
	*/
	virtual void render();

	/*
		освобождает все созданные объекты в классе
	*/
	virtual void shutdown();

	ImGuiRenderer* renderer() const { return m_renderer; }
	/*
		Выводит массив вертексов средствами API движка рендера
	*/
	void renderDrawData(ImDrawData* draw_data);

protected:
	virtual ~ImGuiWrapper();

	/*
		Метод должен быть перегружен. Здесь строятся все примитивы
	*/
	virtual void drawList() = 0;

	const ImVec2& displaySize() { return m_displaySize; }

private:
	/*
		Устанавливает renderstate для вывода 2D графики
	*/
	void setupRenderState(ImDrawData* draw_data);
	/*
		Следит за размером буффера и перевыделяет память при необходимости
	*/
	void checkVertexBuffer(ImDrawData* draw_data);
	/*
		Создает текстуру шрифта
	*/
	bool createFontTexture();
	/*
		Удаляет текстуру шрифта
	*/
	void destroyFontTexture();

private:
	ImVec2 m_displaySize;
	std::string m_fontPath;

	ImGuiRenderer* m_renderer;
	RwRaster* m_fontRaster;

	RwIm2DVertex* m_vertexBuffer;
	int m_vertexBufferSize;
};