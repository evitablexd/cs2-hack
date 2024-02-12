#pragma once
#include <vector>
#include <string>

namespace cfg {
	class config {
	public:
		std::vector<std::string> files_cfg;
	};
	inline const auto g_cfg = std::make_unique<config>();
}

class CSettings
{
public:
	// returns true/false whether the function succeeds, usually returns false if file doesn't exist
	bool Save(std::string file_name);
	void Load(int slot);
	void RemoveConfig(int slot);

	bool CreateConfig(std::string file_name); // creates a blank config

	void GetConfigs();

	bool bunnyhop = false;
	bool nospread = false;
	bool molotov_rendering = false;
	bool smoke_rendering = false;
	bool thirdperson = false;
	float distance = 100.f;
	bool custom_fov = false;
	bool custom_fov_scoped = false;
	float fov = 100.f;
	float zoom_fov = 20.f;
	bool name_esp = false;
	bool box_esp = false;
	bool skin_changer = false;
	int skin_id = 0;
	bool bomb_glow = false;
	bool bomb_text = false;
	bool hitmarker = false;
	bool remove_scope = false;
	bool skeleton_esp = false;
	bool autostrafer = false;
	bool force_crosshair = false;
	bool chams = false;
	bool anti_untrusted = true;
	bool scope_viewmodel = true;
	bool local_chams = false;
	bool bomb_text_fade = true;
	bool healthbar_esp = false;
	bool flags_esp = false;
	s_keybind thirdperson_key;
	s_keybind aimbot_key;
	float aimbot_fov = 50.f;
	int aimbot_bone = 0;
	float aimbot_smoothn = 10.f;
	bool firstperson_leg = true;
	bool ammobar_esp = false;
	bool weapon_text_esp = false;
	bool custom_smoke = false;
	bool draw_distance = false;
	float smoke_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float chams_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float invis_chams_color[4] = { 1.f, 1.f, 1.f, 1.f };
	bool glow = false;
	float glow_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float skeleton_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float hitmaker_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float name_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float box_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float ammo_color[4] = { 95 / 255.f, 174 / 255.f, 227 / 255.f, 1.f };
	bool nightmode = false;
	float nightmode_strength = 100.f;
}; extern CSettings settings;