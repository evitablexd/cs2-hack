#include "../../includes.h"

void Aimbot::Run(CUserCmd* pCmd, CCSGOInputHistoryEntryPB* pInputEntry)
{
	if (!sdk::local_controller || !sdk::local_pawn)
		return;

	if (!sdk::local_controller->m_bPawnIsAlive() || !g_key_binds.get_keybind_state(&settings.aimbot_key))
		return;

	for (int i = 1; i <= interfaces::g_game_resouce->entity_system->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = interfaces::g_game_resouce->entity_system->Get(i);
		if (!entity || !entity->IsBasePlayerController() || entity->IsBasePlayerWeapon()) continue;

		CCSPlayerController* enemy_controller = interfaces::g_game_resouce->entity_system->Get<CCSPlayerController>(entity->GetRefEHandle());
		if (!enemy_controller) { continue; }

		if (enemy_controller == sdk::local_controller || enemy_controller->m_iTeamNum() == sdk::local_controller->m_iTeamNum()) continue;

		C_CSPlayerPawn* enemy_pawn = interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(enemy_controller->m_hPawn());
		if (!enemy_pawn || enemy_pawn == sdk::local_pawn) { continue; }

		// get closest to crosshair

	}
}

C_CSPlayerPawn* Aimbot::GetTarget(CBaseUserCmdPB* pCmd, CCSGOInputHistoryEntryPB* pInputEntry)
{
	return nullptr;
}
