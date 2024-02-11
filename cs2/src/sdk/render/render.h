#pragma once

namespace render
{
	inline ImDrawList* drawList = nullptr;
	inline ImFont* flag_font = nullptr;
	inline ImFont* m_tahoma14 = nullptr;
	inline ImFont* m_acta_symbols = nullptr;
	inline ImFont* m_indicator_font = nullptr;
	inline ImFont* m_verdana = nullptr;

	bool world_to_screen(Vector_t& world, Vector_t& screen);

	Vector2D_t get_text_size(std::string_view txt, ImFont* font);
	void text(std::string_view txt, Vector2D_t pos, const col_t& clr, ImFont* font, bool should_outline, bool should_center_x, bool should_center_y, bool lower_alpha = false, bool drop_shadow = false);
	void rect(const Vector2D_t& pos, const Vector2D_t& size, const col_t& clr, float rounding = 0.f, bool multiplied = false);
	void rect_filled(const Vector2D_t& pos, const Vector2D_t& size, const col_t& clr, float rounding = 0.f, bool multiplied = false);
	void draw_rect(float x1, float y1, float x2, float y2, col_t color);
	void draw_rect_filled(float x, float y, float w, float h, col_t clr, float rounding, ImDrawFlags rounding_corners = 15);
	void line(const Vector2D_t& from, const Vector2D_t& to, const col_t& clr);
	void draw_line(float x1, float y1, float x2, float y2, col_t clr, float thickness = 1.f);
}