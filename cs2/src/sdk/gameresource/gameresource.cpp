#include "../../includes.h"

CGameEntitySystem* CGameEntitySystem::GetInstance()
{
	if (!interfaces::g_game_resouce) return nullptr;

	return interfaces::g_game_resouce->entity_system;
}

CCSPlayerController* CGameEntitySystem::GetLocalPlayerController()
{
	typedef CCSPlayerController* (__fastcall* fn)(int);

	static auto get_local = signature::find("client.dll", "48 83 EC 28 83 F9 FF 75 17 48 8B 0D ? ? ? ?").get<fn>();

	return get_local(-1);
}

void* CGameEntitySystem::GetEntityByIndex(int nIndex)
{
	typedef void* (__fastcall* fn)(void*, int);
	static auto get_base_ent = signature::find("client.dll", "81 FA ? ? ? ? 77 36").get<fn>();

	return get_base_ent(this, nIndex);
}
