#pragma once

struct s_keybind {
    s_keybind()
    {
        m_code = 0;
        m_mode = 0;
        m_is_binding = false;
    }

    s_keybind(char code, int mode)
    {
        m_code = code;
        m_mode = mode;
        m_is_binding = false;
    }

    char m_code;
    int m_mode;
    bool m_is_binding;
};

class c_key_binds
{
public:
    bool get_keybind_state(const s_keybind* bind);
    void set_keybind_state(s_keybind* bind, int val);
    int get_keybind_mode(const s_keybind* bind);
    void add_keybind(const char* label, s_keybind* keybind, bool is_manual = false, int x_bind = 300);

public:
	char m_last_code = NULL;
};

inline c_key_binds g_key_binds;