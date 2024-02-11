#pragma once

inline static constexpr int k_unk_key_hash = 0x31415926;

struct key_string_t {
	__forceinline explicit key_string_t(const std::string_view string, const std::uintptr_t key) :
		m_key(key == 0u ? find_key(string) : key),
		m_string(string.data()) {}

	// particles.dll; 48 89 5C 24 08 57 48 81 EC C0 00 00 00 33 C0 8B
	std::uintptr_t find_key(const std::string_view name);

private:
	std::uintptr_t m_key{ };
	const char* m_string{ };
};

struct material_t {
	__forceinline const char* get_name() noexcept { return CALL_VIRTUAL(const char*, 0, this); }
	__forceinline const char* get_share_name() noexcept { return CALL_VIRTUAL(const char*, 1, this); }
};

struct object_info_t {
	enum e_id : int {
		arm = 38,
		player_t = 104,
		player_ct = 113,
	};
	// todo: id var here (176 offset)
};


class c_object_info
{
public:
	PAD(0xB0);
	int id;
};

class c_scene_animatable_object
{
public:
	char pad_0000[176]; //0x0000
	CBaseHandle owner_handle; //0x00B0
	char pad_00B4[76]; //0x00B4
	void* owner_skeleton; //0x0100
}; //Size: 0x0108
static_assert(sizeof(c_scene_animatable_object) == 0x108);


class material_data_t
{
public:
	char pad_0000[24]; //0x0000
	c_scene_animatable_object* pscene_animatable_object; //0x0018
	material_t* m_material; //0x0020
	char pad_0028[24]; //0x0020
	col_t color; // 0x0040
	c_object_info* object_info; //0x0044
	char pad_0050[24]; //0x0048
};


class CMaterialSystem2 {
public:
	__forceinline material_t*** find_material(material_t*** const material, const std::string_view material_name) noexcept {
		return CALL_VIRTUAL(material_t***, 14, this, material, material_name.data());
	}

	__forceinline material_t** create_material(material_t*** const empty_material, const std::string_view new_material_name, const void* material_data) noexcept {
		return CALL_VIRTUAL(material_t**, 29, this, empty_material, new_material_name.data(), material_data, 0, 0, 0, 0, 0, 1);
	}

	__forceinline void set_create_data_by_material(const void* data, material_t*** const material) noexcept {
		return CALL_VIRTUAL(void, 37, this, material, data);
	}
};