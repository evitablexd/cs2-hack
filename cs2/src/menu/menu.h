#pragma once

class variables
{
public:
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
	float name_color[4] = { 1.f, 1.f, 1.f, 1.f };
	float box_color[4] = { 1.f, 1.f, 1.f, 1.f };
	bool nightmode = false;
	float nightmode_strength = 100.f;
};

inline variables g_variables;

class menu
{
private:
	bool open = true;
	std::shared_ptr<xpui::form> main_window = nullptr;

public:

	bool& is_opened() { return this->open; }

	void render();
};

inline menu g_menu;