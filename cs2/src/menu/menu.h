#pragma once



class menu
{
private:
	bool open = true;
	std::shared_ptr<xpui::form> main_window = nullptr;

	int slot_cfg = 0;
	char config_name[256];
	static std::string filter;
	std::string new_cfg;

public:

	bool& is_opened() { return this->open; }

	void render();
};

inline menu g_menu;