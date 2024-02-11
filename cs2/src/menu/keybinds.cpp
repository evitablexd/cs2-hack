#include "../includes.h"

bool c_key_binds::get_keybind_state(const s_keybind* bind)
{
	if (bind->m_code == VK_ESCAPE) {
		return false;
	}

	switch (bind->m_mode)
	{
	case 0: return bind->m_code && GetAsyncKeyState(bind->m_code); break;
	case 1: return bind->m_code && GetKeyState(bind->m_code); break;
	case 2: return true; break;
	case 3: return false; break;
	}

	return false;
}


int c_key_binds::get_keybind_mode(const s_keybind* bind) {
	return bind->m_mode;
}

void c_key_binds::set_keybind_state(s_keybind* bind, int val) {
	bind->m_mode = val;
}

inline const char* Keys__[] = {
"[-]",
"[M1]",
"[M2]",
"[CN]",
"[M3]",
"[M4]",
"[M5]",
"[-]",
"[BAC]",
"[TAB]",
"[-]",
"[-]",
"[CLR]",
"[RET]",
"[-]",
"[-]",
"[SHI]",
"[CTL]",
"[ALT]",
"[PAU]",
"[CAP]",
"[KAN]",
"[-]",
"[JUN]",
"[FIN]",
"[KAN]",
"[-]",
"[ESC]",
"[CON]",
"[NCO]",
"[ACC]",
"[MAD]",
"[SPA]",
"[PGU]",
"[PGD]",
"[END]",
"[HOM]",
"[LEF]",
"[UP]",
"[RIG]",
"[DOW]",
"[SEL]",
"[PRI]",
"[EXE]",
"[PRI]",
"[INS]",
"[DEL]",
"[HEL]",
"[0]",
"[1]",
"[2]",
"[3]",
"[4]",
"[5]",
"[6]",
"[7]",
"[8]",
"[9]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[A]",
"[B]",
"[C]",
"[D]",
"[E]",
"[F]",
"[G]",
"[H]",
"[I]",
"[J]",
"[K]",
"[L]",
"[M]",
"[N]",
"[O]",
"[P]",
"[Q]",
"[R]",
"[S]",
"[T]",
"[U]",
"[V]",
"[W]",
"[X]",
"[Y]",
"[Z]",
"[WIN]",
"[WIN]",
"[APP]",
"[-]",
"[SLE]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[NUM]",
"[MUL]",
"[ADD]",
"[SEP]",
"[MIN]",
"[DEC]",
"[DIV]",
"[F1]",
"[F2]",
"[F3]",
"[F4]",
"[F5]",
"[F6]",
"[F7]",
"[F8]",
"[F9]",
"[F10]",
"[F11]",
"[F12]",
"[F13]",
"[F14]",
"[F15]",
"[F16]",
"[F17]",
"[F18]",
"[F19]",
"[F20]",
"[F21]",
"[F22]",
"[F23]",
"[F24]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[NUM]",
"[SCR]",
"[EQU]",
"[MAS]",
"[TOY]",
"[OYA]",
"[OYA]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[-]",
"[SHI]",
"[SHI]",
"[CTR]",
"[CTR]",
"[ALT]",
"[ALT]"
};
const char* modes[] = { "Hold", "Toggle", "Always on", "Always off" };

void c_key_binds::add_keybind(const char* label, s_keybind* keybind, bool is_manual, int x_bind)
{
	if (!is_manual) {
		ImGui::PushItemWidth(60.f);
		ImGui::Combo(label, &keybind->m_mode, modes, IM_ARRAYSIZE(modes));
		ImGui::PopItemWidth();
	}
	else {
		ImGui::Text(label);
		keybind->m_mode = 0;
	}
	std::string ButtonString;

	if (keybind->m_code == VK_ESCAPE) {
		ButtonString = ("-");
	}
	else if (keybind->m_is_binding)
		ButtonString = ("...");
	else if (!keybind->m_code)
		ButtonString = ("-");
	else
	{
		ButtonString = Keys__[keybind->m_code];
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX(x_bind);
	std::string str = std::string(ButtonString + ("###BindButton_") + label);

	auto len_ = str.length();

	if (len_ >= 4)
		len_ += 9;

	if (ImGui::Button((ButtonString + std::string(("###BindButton_")) + label).c_str(), ImVec2(len_, 18)))
	{
		keybind->m_is_binding = true;
		m_last_code = 0;
	}

	//gui::SetCursorPosY( last_y_pos );

	if (keybind->m_is_binding && m_last_code)
	{
		keybind->m_code = m_last_code;
		keybind->m_is_binding = false;
		m_last_code = 0;
	}
}