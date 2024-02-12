#include "includes.h"

float get_absolute_time()
{
	return (float)(clock() / (float)1000.f);
}

void c_logs::draw_data()
{
	for (int i = 0u; i < m_logs.size(); i++)
	{
		log_data_t* log_data = &m_logs[i];
		if (!log_data)
			continue;

		float_t time_delta = get_absolute_time() - log_data->m_creation_time;

		if (time_delta >= 2.65f
			|| m_logs.size() > 10.f)
		{
			m_logs.erase(m_logs.begin() + i);
			continue;
		}

		auto size = render::m_verdana->CalcTextSizeA(12.f, FLT_MAX, NULL, log_data->m_string.c_str());

		if (time_delta >= 2.5f)
		{
			log_data->m_text_alpha = std::lerp(log_data->m_text_alpha, 0.f, 18.f * interfaces::g_global_vars->m_flFrameTime);
			log_data->m_spacing = std::lerp(log_data->m_spacing, -size.x * 2.f, 8.f * interfaces::g_global_vars->m_flFrameTime);
		}
		else
		{
			log_data->m_text_alpha = std::lerp(log_data->m_text_alpha, 1.f, 9.f * interfaces::g_global_vars->m_flFrameTime);;
			log_data->m_spacing = std::lerp(log_data->m_spacing, 4.f, 16.f * interfaces::g_global_vars->m_flFrameTime);
		}

		constexpr uint8_t white_clr[4] = { 225, 225, 225, 225 };
		constexpr uint8_t blue_clr[4] = { 245, 245, 245, 225 };

		/*if (!log_data->m_printed)
		{
			game::g_cvar->con_print(false, *blue_clr, xor_str("[negus] "));
			game::g_cvar->con_print(false, *white_clr, log_data->m_string.c_str());
			log_data->m_printed = true;
		}*/

		render::text(log_data->m_string, Vector2D_t(log_data->m_spacing, size.y * i),
			col_t(log_data->m_color.r(), log_data->m_color.g(), log_data->m_color.b(),
				(int)(255.f * log_data->m_text_alpha)), render::m_verdana, false, false, false, false, true);
	}
}

void c_logs::push_log(std::string log, col_t color)
{
	log_data_t data;

	data.m_creation_time = get_absolute_time();
	data.m_spacing = -160.f;
	data.m_text_alpha = 0.f;
	data.m_string = log;
	data.m_color = color;

	m_logs.emplace_back(data);
}
