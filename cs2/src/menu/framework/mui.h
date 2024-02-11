#pragma once

namespace xpui
{
	class child
	{
	private:
		std::string child_name{};
		Vector2D_t child_size{};

	public:
		child() { }
		child(std::string name, Vector2D_t size) { this->child_name = name; this->child_size = size; }
	};

	class tab
	{
	private:
		std::string tab_name{};

	public:
		tab() { }
		tab(std::string name) { this->tab_name = name; }

		child* add_child(std::string name, Vector2D_t size);

	};

	class form
	{
	private:
		std::string form_name{};
		Vector2D_t form_size{};

	public:
		form() { }
		form(std::string name, Vector2D_t size);

		tab* add_tab(std::string name);

	};
}