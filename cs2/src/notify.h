#pragma once

#include <deque>

struct log_data_t {
	float m_creation_time = 0.f;
	float m_text_alpha = 0.0f;
	float m_spacing = 0.0f;

	std::string m_string = "";

	col_t m_color;

	bool m_printed = false;
};

class c_logs {
public:
	void draw_data();
	void push_log(std::string log, col_t color);

private:
	std::deque < log_data_t > m_logs;
};

inline c_logs g_logs;