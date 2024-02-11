#include "../includes.h"

void menu::render()
{
	//this->main_window = std::make_shared<xpui::form>("test", Vector2D_t(500, 300));


	ImGui::PushFont(ImGui::GetDefaultFont());

	ImGui::Checkbox("Nospread", &g_variables.nospread);
	ImGui::Separator();

	ImGui::Checkbox("Bunnyhop", &g_variables.bunnyhop);
	ImGui::Checkbox("Auto-strafer", &g_variables.autostrafer);

	ImGui::Separator();

	g_key_binds.add_keybind("Thirdperson", &g_variables.thirdperson_key);
	ImGui::SliderFloat("Distance", &g_variables.distance, 1.f, 150.f);

	ImGui::Checkbox("Custom Fov", &g_variables.custom_fov);
	if (g_variables.custom_fov) {
		ImGui::Checkbox("^ While scoped", &g_variables.custom_fov_scoped);
		ImGui::SliderFloat("Fov", &g_variables.fov, 1.f, 150.f);
		ImGui::SliderFloat("Zoom Fov", &g_variables.zoom_fov, 1.f, 150.f);
	}

	ImGui::Separator();

	ImGui::Checkbox("Show names", &g_variables.name_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##names_color", g_variables.name_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Show boxes", &g_variables.box_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##boxes_color", g_variables.box_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Show health", &g_variables.healthbar_esp);
	ImGui::Checkbox("Show ammo", &g_variables.ammobar_esp);
	ImGui::Checkbox("Show distance", &g_variables.draw_distance);
	ImGui::Checkbox("Show weapon text", &g_variables.weapon_text_esp);
	ImGui::Checkbox("Show flags", &g_variables.flags_esp);
	ImGui::Checkbox("Show skeleton", &g_variables.skeleton_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##skeleton_color", g_variables.skeleton_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Remove molotov", &g_variables.molotov_rendering);
	ImGui::Checkbox("Remove smoke", &g_variables.smoke_rendering);
	ImGui::Checkbox("Remove scope", &g_variables.remove_scope);
	ImGui::Checkbox("Glow", &g_variables.glow); ImGui::SameLine();
	ImGui::ColorEdit4("##glow_color", g_variables.glow_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Chams", &g_variables.chams); ImGui::SameLine();
	ImGui::ColorEdit4("##chams_color", g_variables.chams_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar); ImGui::SameLine();
	ImGui::ColorEdit4("##invis_color", g_variables.invis_chams_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Local Chams", &g_variables.local_chams);


	ImGui::Separator();
	ImGui::Checkbox("Custom Smoke Color", &g_variables.custom_smoke); ImGui::SameLine();
	ImGui::ColorEdit4("##smoke_color", g_variables.smoke_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Nightmode", &g_variables.nightmode);
	ImGui::SliderFloat("##strength", &g_variables.nightmode_strength, 1.f, 100.f);
	ImGui::Checkbox("Remove first person legs", &g_variables.firstperson_leg);
	ImGui::Checkbox("Bomb glow", &g_variables.bomb_glow);
	ImGui::Checkbox("Bomb text", &g_variables.bomb_text);
	ImGui::Checkbox("^ Fade based on distance", &g_variables.bomb_text_fade);
	ImGui::Checkbox("Force crosshair", &g_variables.force_crosshair);
	ImGui::Checkbox("Show viewmodel while scoping", &g_variables.scope_viewmodel);

	ImGui::Separator();

	ImGui::Checkbox("Skin changer", &g_variables.skin_changer);
	ImGui::SliderInt("##skin_id", &g_variables.skin_id, 0, 1000);

	ImGui::Separator();

	ImGui::Checkbox("Anti-Untrusted", &g_variables.anti_untrusted);
	if (ImGui::Button("save", ImVec2(30, 20)))
		g_cfg.save("test.cfg");

	if (ImGui::Button("load", ImVec2(30, 20)))
		g_cfg.load("test.cfg");

	ImGui::PopFont();
}
