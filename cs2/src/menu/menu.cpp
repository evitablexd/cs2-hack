#include "../includes.h"

void menu::render()
{
	//this->main_window = std::make_shared<xpui::form>("test", Vector2D_t(500, 300));


	ImGui::PushFont(ImGui::GetDefaultFont());

	ImGui::Checkbox("Nospread", &settings.nospread);
	ImGui::Separator();

	ImGui::Checkbox("Bunnyhop", &settings.bunnyhop);
	ImGui::Checkbox("Auto-strafer", &settings.autostrafer);

	ImGui::Separator();

	g_key_binds.add_keybind("Thirdperson", &settings.thirdperson_key);
	ImGui::SliderFloat("Distance", &settings.distance, 1.f, 150.f);

	ImGui::Checkbox("Custom Fov", &settings.custom_fov);
	if (settings.custom_fov) {
		ImGui::Checkbox("^ While scoped", &settings.custom_fov_scoped);
		ImGui::SliderFloat("Fov", &settings.fov, 1.f, 150.f);
		ImGui::SliderFloat("Zoom Fov", &settings.zoom_fov, 1.f, 150.f);
	}

	ImGui::Separator();

	ImGui::Checkbox("Show names", &settings.name_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##names_color", settings.name_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Show boxes", &settings.box_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##boxes_color", settings.box_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Show health", &settings.healthbar_esp);
	ImGui::Checkbox("Show ammo", &settings.ammobar_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##ammo_color", settings.ammo_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Show distance", &settings.draw_distance);
	ImGui::Checkbox("Show weapon text", &settings.weapon_text_esp);
	ImGui::Checkbox("Show flags", &settings.flags_esp);
	ImGui::Checkbox("Show skeleton", &settings.skeleton_esp); ImGui::SameLine();
	ImGui::ColorEdit4("##skeleton_color", settings.skeleton_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Checkbox("Remove molotov", &settings.molotov_rendering);
	ImGui::Checkbox("Remove smoke", &settings.smoke_rendering);
	ImGui::Checkbox("Remove scope", &settings.remove_scope);
	ImGui::Checkbox("Glow", &settings.glow); ImGui::SameLine();
	ImGui::ColorEdit4("##glow_color", settings.glow_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Chams", &settings.chams); ImGui::SameLine();
	ImGui::ColorEdit4("##chams_color", settings.chams_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar); ImGui::SameLine();
	ImGui::ColorEdit4("##invis_color", settings.invis_chams_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Local Chams", &settings.local_chams);


	ImGui::Separator();
	ImGui::Checkbox("Custom Smoke Color", &settings.custom_smoke); ImGui::SameLine();
	ImGui::ColorEdit4("##smoke_color", settings.smoke_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
	ImGui::Checkbox("Nightmode", &settings.nightmode);
	ImGui::SliderFloat("##strength", &settings.nightmode_strength, 1.f, 100.f);
	ImGui::Checkbox("Remove first person legs", &settings.firstperson_leg);
	ImGui::Checkbox("Bomb glow", &settings.bomb_glow);
	ImGui::Checkbox("Bomb text", &settings.bomb_text);
	ImGui::Checkbox("^ Fade based on distance", &settings.bomb_text_fade);
	ImGui::Checkbox("Force crosshair", &settings.force_crosshair);
	//ImGui::Checkbox("Show viewmodel while scoping", &g_variables.scope_viewmodel);
	ImGui::Checkbox("Hitmarker", &settings.hitmarker); ImGui::SameLine();
	ImGui::ColorEdit4("##hitmarker_color", settings.hitmaker_color, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

	ImGui::Separator();

	ImGui::Checkbox("Skin changer", &settings.skin_changer);
	ImGui::SliderInt("##skin_id", &settings.skin_id, 0, 1000);

	ImGui::Separator();

	ImGui::Checkbox("Anti-Untrusted", &settings.anti_untrusted);

	std::vector<const char*> items;
	items.reserve(cfg::g_cfg->files_cfg.size());

	for (const auto& item : cfg::g_cfg->files_cfg) {
		items.push_back(item.c_str());
	}

	ImGui::ListBox("configs", &this->slot_cfg, items.data(), static_cast<int>(items.size()));
	ImGui::InputText("config name", this->config_name, sizeof(this->config_name));

	settings.GetConfigs();

	if (ImGui::Button("save config", ImVec2(70, 20))) {
		if (!settings.CreateConfig(this->config_name))
			return;

		settings.Save(this->config_name);
		settings.GetConfigs();
	}

	if (ImGui::Button("load config", ImVec2(70, 20))) {
		if (!cfg::g_cfg->files_cfg.size())
			return;

		settings.Load(this->slot_cfg);
	}

	if (ImGui::Button("delete config", ImVec2(70, 20))) {
		if (!cfg::g_cfg->files_cfg.size())
			return;

		settings.RemoveConfig(this->slot_cfg);
	}


	ImGui::PopFont();
}
