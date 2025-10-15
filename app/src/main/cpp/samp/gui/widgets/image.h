#pragma once

class Image : public Widget
{
public:
	Image(const std::string& arch_name, const std::string& name);

	virtual void performLayout() override;
	virtual void draw(ImGuiRenderer* renderer) override;

private:
	RwTexture* m_texture;
};