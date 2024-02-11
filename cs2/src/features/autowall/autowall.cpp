#include "../../includes.h"

void c_auto_wall::scale_damage(const int hitgroup, C_BaseEntity* entity)
{
    static ConVar* mp_damage_scale_ct_head = interfaces::g_cvar->FindVarByName("mp_damage_scale_ct_head"),
        * mp_damage_scale_t_head = interfaces::g_cvar->FindVarByName("mp_damage_scale_t_head"),
        * mp_damage_scale_ct_body = interfaces::g_cvar->FindVarByName("mp_damage_scale_ct_body"),
        * mp_damage_scale_t_body = interfaces::g_cvar->FindVarByName("mp_damage_scale_t_body");

    const auto damage_scale_ct_head = mp_damage_scale_ct_head->GetValue< float >(),
        damage_scale_t_head = mp_damage_scale_t_head->GetValue< float >(),
        damage_scale_ct_body = mp_damage_scale_ct_body->GetValue< float >(),
        damage_scale_t_body = mp_damage_scale_t_body->GetValue< float >();

    const bool is_ct = entity->m_iTeamNum() == 3, is_t = entity->m_iTeamNum() == 2;

    float head_damage_scale = is_ct ? damage_scale_ct_head : is_t ? damage_scale_t_head : 1.0f;
    const float body_damage_scale = is_ct ? damage_scale_ct_body : is_t ? damage_scale_t_body : 1.0f;

}

bool c_auto_wall::fire_bullet()
{
    return false;
}