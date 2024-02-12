#include "../includes.h"
#include <fstream>

bool CSettings::Save(std::string file_name)
{
	std::string file_path = "C:\\cs2hack\\" + file_name + ".cfg";

	std::fstream file(file_path, std::ios::out | std::ios::in | std::ios::trunc);
	file.close();

	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		g_logs.push_log(tfm::format("Failed to save config: %s", file_name), col_t(255, 255, 255, 255));
		return false;
	}

	const size_t settings_size = sizeof(CSettings);
	for (int i = 0; i < settings_size; i++)
	{
		byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
		for (int x = 0; x < 8; x++)
		{
			file << (int)((current_byte >> x) & 1);
		}
	}

	file.close();

	g_logs.push_log(tfm::format("Saved config: %s", file_name), col_t(255, 255, 255, 255));

	return true;
}

void CSettings::Load(int slot)
{
	CreateDirectory("C:\\cs2hack", NULL);

	std::string file_path = "C:\\cs2hack\\" + cfg::g_cfg->files_cfg[slot] + ".cfg";

	std::fstream file;
	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		g_logs.push_log(tfm::format("Failed to load config: %s", cfg::g_cfg->files_cfg[slot].c_str()), col_t(255, 255, 255, 255));
		return;
	}

	std::string line;
	while (file)
	{
		std::getline(file, line);

		const size_t settings_size = sizeof(CSettings);
		if (line.size() > settings_size * 8)
		{
			file.close();
			return;
		}
		for (int i = 0; i < settings_size; i++)
		{
			byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
			for (int x = 0; x < 8; x++)
			{
				if (line[(i * 8) + x] == '1')
					current_byte |= 1 << x;
				else 
					current_byte &= ~(1 << x);
			}
			*reinterpret_cast<byte*>(uintptr_t(this) + i) = current_byte;
		}
	}

	file.close();

	g_logs.push_log(tfm::format("Loaded config: %s", cfg::g_cfg->files_cfg[slot]), col_t(255, 255, 255, 255));
}

void CSettings::RemoveConfig(int slot)
{
	std::string file_path = "C:\\cs2hack\\" + cfg::g_cfg->files_cfg[slot] + ".cfg";

	// attempt to open the file for removal.
	std::fstream file(file_path, std::ios::in);
	if (!file.is_open()) {
		// file doesn't exist, or cannot be opened for removal.
		g_logs.push_log(tfm::format("Failed to open config: %s", cfg::g_cfg->files_cfg[slot]), col_t(255, 255, 255, 255));
		return;
	}

	// close the file.
	file.close();

	// remove the file.
	if (std::remove(file_path.c_str()) != 0) {
		// error occurred while removing the file.
		g_logs.push_log(tfm::format("Failed to remove config: %s", cfg::g_cfg->files_cfg[slot]), col_t(255, 255, 255, 255));
		return;
	}

	g_logs.push_log(tfm::format("Removed config: %s", cfg::g_cfg->files_cfg[slot]), col_t(255, 255, 255, 255));
}

bool CSettings::CreateConfig(std::string file_name)
{
	std::string file_path = "C:\\cs2hack\\" + file_name + ".cfg";

	std::fstream file(file_path, std::ios::out | std::ios::in | std::ios::trunc);
	file.close();

	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		//g_logs.push_log(tfm::format("Failed to create config: %s", file_name.c_str()), col_t(255, 255, 255, 255));
		return false;
	}
	file.close();

	//g_logs.push_log(tfm::format("Created config: %s", file_name.c_str()), col_t(255, 255, 255, 255));

	return true;
}

void CSettings::GetConfigs()
{
	WIN32_FIND_DATA ffd;
	auto directory = "C:\\cs2hack\\*";
	auto hFind = FindFirstFile(directory, &ffd);

	cfg::g_cfg->files_cfg.clear();

	while (FindNextFile(hFind, &ffd))
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::string file_name = ffd.cFileName;
			if (file_name.size() < 4) // .cfg
				continue;

			std::string end = file_name;
			end.erase(end.begin(), end.end() - 4); // erase everything but the last 4 letters
			if (end != ".cfg")
				continue;

			file_name.erase(file_name.end() - 4, file_name.end()); // erase the .cfg part
			cfg::g_cfg->files_cfg.push_back(file_name);
		}
	}
}

CSettings settings;