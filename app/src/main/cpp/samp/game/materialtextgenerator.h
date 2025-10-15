#pragma once

#define OBJECT_MATERIAL_SIZE_32x32  10
#define OBJECT_MATERIAL_SIZE_64x32  20
#define OBJECT_MATERIAL_SIZE_64x64  30
#define OBJECT_MATERIAL_SIZE_128x32  40
#define OBJECT_MATERIAL_SIZE_128x64  50
#define OBJECT_MATERIAL_SIZE_128x128  60
#define OBJECT_MATERIAL_SIZE_256x32  70
#define OBJECT_MATERIAL_SIZE_256x64  80
#define OBJECT_MATERIAL_SIZE_256x128  90
#define OBJECT_MATERIAL_SIZE_256x256  100
#define OBJECT_MATERIAL_SIZE_512x64  110
#define OBJECT_MATERIAL_SIZE_512x128  120
#define OBJECT_MATERIAL_SIZE_512x256  130
#define OBJECT_MATERIAL_SIZE_512x512  140

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../vendor/imgui/imgui.h"

class MaterialTextGenerator
{
public:
	MaterialTextGenerator();

    RwTexture* Generate(const char* text, int size, int font_size, bool bold, uint32_t font_color, uint32_t background_color, int alignment);

private:
	void SetUpScene();
	void Render(const char* text, const ImVec2& size, int font_size, bool bold,
		const ImColor& font_color, const ImColor& background_color, int align);
	void GetMaterialSize(int size, int* x, int* y);

    RwCamera* m_camera;
	RwFrame* m_frame;
};