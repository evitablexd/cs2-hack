#include "../../includes.h"

CCSPlayerController* CCSPlayerController::GetLocalPlayerController() noexcept
{
    const int nIndex = interfaces::g_engine->GetLocalPlayer();
    return interfaces::g_game_resouce->entity_system->Get<CCSPlayerController>(nIndex);
}