#include "../../includes.h"

xpui::form::form(std::string name, Vector2D_t size)
{

    if (g_menu.is_opened()) {
        ImGui::SetNextWindowPos(size.ToVec2());
        ImGui::SetNextWindowSize(size.ToVec2());
        ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

        auto drawlist = ImGui::GetWindowDrawList();

        // start position for rect
       // Vector2D_t x_rect_start = Vector2D_t(size.x, size.y - 10);
       // Vector2D_t x_rect_end = Vector2D_t(size.x + 10, size.y + 10);

        // background
        drawlist->AddRectFilled(ImVec2(0, 0), size.ToVec2(), ImColor(5, 5, 5, 200), 10.f, ImDrawFlags_RoundCornersAll);

        ImGui::End();
    }
}

xpui::tab* xpui::form::add_tab(std::string name)
{


    return nullptr;
}

xpui::child* xpui::tab::add_child(std::string name, Vector2D_t size)
{
    return nullptr;
}
