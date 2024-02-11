#pragma once

class c_cfg
{
private:
	std::vector<std::uint8_t> vars;
public:
	static constexpr std::uint8_t k_byte_xor = 0xffu;

public:
	void save(const std::string_view name) const;

	void load(const std::string_view name);
};

inline c_cfg g_cfg;