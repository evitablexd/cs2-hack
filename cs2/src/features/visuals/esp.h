#pragma once

struct flags_data_t {
	std::string m_name{ };
	col_t m_clr{ };
};

namespace esp
{
	RECT get_bbox(C_BasePlayerPawn* ent, bool is_valid);

	void render();
	void render_fsn(int stage);

	material_t* create_material(const std::string_view material_name, bool ignore_z, bool translucent, bool blend);
}

namespace chams
{
	

	inline bool bInitialized = false;
}