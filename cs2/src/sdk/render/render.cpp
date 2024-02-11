#include "../../includes.h"

bool render::world_to_screen(Vector_t& origin, Vector_t& out)
{
	static auto screen_transform = signature::find("client.dll", "48 89 74 24 ? 57 48 83 EC 20 48 8B 05 ? ? ? ? 48 8B FA").get<bool(__fastcall*)(Vector_t&, Vector_t&) >();

	if (!screen_transform)
		return false;

	auto ret = !(screen_transform(origin, out));

	out.x = ((out.x + 1.0f) * 0.5f) * ImGui::GetIO().DisplaySize.x;
	out.y = ImGui::GetIO().DisplaySize.y - (((out.y + 1.0f) * 0.5f) * ImGui::GetIO().DisplaySize.y);

	return ret;
}

Vector2D_t render::get_text_size(std::string_view txt, ImFont* font)
{
	if (!font
		|| txt.empty()
		|| !font->IsLoaded())
		return Vector2D_t();

	const auto size = font->CalcTextSizeA(font->FontSize, std::numeric_limits<float>::max(), 0.f, txt.data());

	return Vector2D_t(IM_FLOOR(size.x + 0.95f), size.y);
}

void render::text(std::string_view txt, Vector2D_t pos, const col_t& clr, ImFont* font, bool should_outline, bool should_center_x, bool should_center_y, bool lower_alpha, bool drop_shadow)
{
	if (!font
		|| txt.empty()
		|| clr.a() <= 0
		|| !font->IsLoaded())
		return;

	const auto centered_x = should_center_x;
	const auto centered_y = should_center_y;

	if (centered_x
		|| centered_y) {
		const auto text_size = get_text_size(txt, font);

		if (centered_x) {
			pos.x -= text_size.x / 2.f;
		}

		if (centered_y) {
			pos.y -= text_size.y / 2.f;
		}
	}

	drawList->PushTextureID(font->ContainerAtlas->TexID);

	auto outline_alpha = std::move(clr.a() / 1.5f);

	if (lower_alpha)
		outline_alpha = std::move(clr.a() / 2.5f);

	if (should_outline) {
		drawList->AddTextOutline(font, font->FontSize, *reinterpret_cast<ImVec2*> (&pos), clr.hex(), txt.data(), 0, outline_alpha);
	}
	else {
		if (drop_shadow) {
			drawList->AddText(font, font->FontSize, ImVec2(pos.x + 1, pos.y + 1), col_t(0.f, 0.f, 0.f, outline_alpha).hex(), txt.data());
		}
		drawList->AddText(font, font->FontSize, *reinterpret_cast<ImVec2*> (&pos), clr.hex(), txt.data());
	}

	drawList->PopTextureID();
}

void render::rect(const Vector2D_t& pos, const Vector2D_t& size, const col_t& clr, float rounding, bool multiplied)
{
	drawList->AddRect(ImVec2(pos.x, pos.y), multiplied ? ImVec2(pos.x + size.x, pos.y + size.y) : ImVec2(size.x, size.y), clr.hex(), rounding, 15, 0.3f);
}

void render::rect_filled(const Vector2D_t& pos, const Vector2D_t& size, const col_t& clr, float rounding, bool multiplied)
{
	drawList->AddRectFilled(ImVec2(pos.x, pos.y), multiplied ? ImVec2(pos.x + size.x, pos.y + size.y) : ImVec2(size.x, size.y), clr.hex(), rounding);
}

void render::draw_rect(float x1, float y1, float x2, float y2, col_t color)
{
	drawList->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), color.hex(), 0.0f);
}

void render::draw_rect_filled(float x, float y, float w, float h, col_t clr, float rounding, ImDrawFlags rounding_corners)
{
	drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), clr.hex(), rounding, rounding_corners);
}

void render::line(const Vector2D_t& from, const Vector2D_t& to, const col_t& clr)
{
	drawList->AddLine(*reinterpret_cast<const ImVec2*> (&from), *reinterpret_cast<const ImVec2*> (&to), clr.hex());
}

void render::draw_line(float x1, float y1, float x2, float y2, col_t clr, float thickness)
{
	drawList->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), clr.hex(), thickness);
}
