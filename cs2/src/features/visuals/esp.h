#pragma once

struct flags_data_t {
	std::string m_name{ };
	col_t m_clr{ };
};

struct hit_marker_data_t {
	float m_spawn_time{ };
	float m_alpha{ };
};

namespace esp
{
	__forceinline const char* GetWeaponName(int ID) {
		switch (ID) {
		case 0: return "none";
		case 1: return "deagle";
		case 2: return "elite";
		case 3: return "fiveseven";
		case 4: return "glock";
		case 7: return "ak47";
		case 8: return "aug";
		case 9: return "awp";
		case 10: return "famas";
		case 11: return "g3sg1";
		case 13: return "galilar";
		case 14: return "ms49";
		case 16: return "m4a1";
		case 17: return "mac10";
		case 19: return "p90";
		case 23: return "mp5sd";
		case 24: return "ump45";
		case 25: return "xm1014";
		case 26: return "bizon";
		case 27: return "mag7";
		case 28: return "negev";
		case 29: return "sawedoff";
		case 30: return "tec9";
		case 31: return "taser";
		case 32: return "hkp2000";
		case 33: return "mp7";
		case 34: return "mp9";
		case 35: return "nova";
		case 36: return "p250";
		case 37: return "shield";
		case 38: return "scar20";
		case 39: return "sg556";
		case 40: return "ssg08";
		case 41: return "knifegg";
		case 42: return "knife";
		case 43: return "flashbang";
		case 44: return "hegrenade";
		case 45: return "smoke";
		case 46: return "molotov";
		case 47: return "decoy";
		case 48: return "incendiary";
		case 49: return "c4";
		case 57: return "healthshot";
		case 59: return "knife_t";
		case 60: return "m4a1_silencer";
		case 61: return "usp_silencer";
		case 63: return "cz75a";
		case 64: return "revolver";
		case 68: return "tagrenade";
		case 69: return "fists";
		case 70: return "breachcharge";
		case 72: return "tablet";
		case 74: return "melee";
		case 75: return "axe";
		case 76: return "hammer";
		case 78: return "spanner";
		case 80: return "knife_ghost";
		case 81: return "firebomb";
		case 82: return "diversion";
		case 83: return "frag_grenade";
		case 84: return "snowball";
		case 85: return "bumpmine";
		case 500: return "bayonet";
		case 503: return "css";
		case 505: return "flip";
		case 506: return "gut";
		case 507: return "karambit";
		case 508: return "m9_bayonet";
		case 509: return "tactical";
		case 512: return "falchion";
		case 514: return "survival_bowie";
		case 515: return "butterfly";
		case 516: return "push";
		case 517: return "cord";
		case 518: return "canis";
		case 519: return "ursus";
		case 520: return "gypsy_jackknife";
		case 521: return "outdoor";
		case 522: return "stiletto";
		case 523: return "widowmaker";
		case 525: return "skeleton";
		case 52428: return "p2000";
		case 5027: return "STUDDED_BLOODHOUND_GLOVES";
		case 5028: return "T_GLOVES";
		case 5029: return "CT_GLOVES";
		case 5030: return "SPORTY_GLOVES";
		case 5031: return "SLICK_GLOVES";
		case 5032: return "LEATHER_HANDWRAPS";
		case 5033: return "MOTORCYCLE_GLOVES";
		case 5034: return "SPECIALIST_GLOVES";
		case 5035: return "STUDDED_HYDRA_GLOVES";
		default: return "unknown";
		}
	}

	RECT get_bbox(C_BasePlayerPawn* ent, bool is_valid);

	void render();
	void render_fsn(int stage);
	void render_hitmarker();

	material_t* create_material(const std::string_view material_name, bool ignore_z, bool translucent, bool blend);

	inline std::deque < hit_marker_data_t > m_hit_markers{ };
}