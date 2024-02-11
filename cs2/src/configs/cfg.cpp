#include "../includes.h"

void c_cfg::save(const std::string_view name) const
{
	const auto path = std::filesystem::path{ "C:\\configs\\" } /= name;

	std::filesystem::create_directory("C:\\configs");

	nlohmann::json json{ };

	/* use a new object to verify our cfg on the load */
	auto& object = json[("configs")];

	object.emplace_back(g_variables.box_esp);
	object.emplace_back(g_variables.name_esp);
	object.emplace_back(g_variables.bunnyhop);
	object.emplace_back(g_variables.nospread);

	auto str = json.dump();

	/* encrypt our cfg so the user wouldn't know tf stored in it */
	/* there is no need of more complex algorithms, basic xor would be enough */
	//for (auto& chr : str)
	//	chr ^= k_byte_xor;

	// write file to path
	if (std::ofstream file{ path, std::ios::out | std::ios::trunc })
		file << str;
}

void c_cfg::load(const std::string_view name)
{
	const auto path = std::filesystem::path{ "C:\\configs\\" } /= name;

	std::filesystem::create_directory("C:\\configs");

	std::string str{ };

	// read file in the path based on the name
	if (std::ifstream file{ path, std::ios::in })
		file >> str;

	if (str.empty())
		return;

	/* decrypt cfg */
	//for (auto& chr : str)
	//	chr ^= k_byte_xor;

	const auto json = nlohmann::json::parse(str);
	if (!json.is_object())
		return;

	/* verify cfg file via trying to find our identity object */
	const auto arr = json.find(("configs"));
	if (arr == json.end())
		return;

	const auto& bytes = arr.value();

	for (const auto& chr : bytes.items())
	{

	}

}
