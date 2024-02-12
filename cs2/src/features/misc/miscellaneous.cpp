#include "../../includes.h"

// movement correction angles
static QAngle_t angCorrectionView = {};

void miscellaneous::run(CUserCmd* cmd)
{
	CBaseUserCmdPB* pBaseCmd = cmd->pBaseCmd;
	if (pBaseCmd == nullptr)
		return;

	bunny_hop(cmd);
	auto_strafe(cmd);

	// loop through all tick commands
	for (int tick = 0; tick < pBaseCmd->nTickCount; tick++)
	{
		CCSGOInputHistoryEntryPB* pInputEntry = cmd->csgoUserCmd.GetInputHistoryEntry(tick);
		if (pInputEntry == nullptr)
			continue;

		// save view angles for movement correction
		angCorrectionView = pInputEntry->pViewCmd->angValue;

		// movement correction & anti-untrusted
		validate_user_cmd(cmd, pBaseCmd, pInputEntry);
	}
}

void miscellaneous::bunny_hop(CUserCmd* cmd)
{
	if (!sdk::local_controller->m_bPawnIsAlive() || !settings.bunnyhop)
		return;

	// check if player is in noclip or on ladder
	if (const int32_t movetype = sdk::local_pawn->m_MoveType(); movetype == MOVETYPE_NOCLIP || movetype == MOVETYPE_LADDER)
		return;

	if (sdk::local_pawn->m_fFlags() & FL_ONGROUND)
		cmd->nButtons.nValue &= ~IN_JUMP;
}

void miscellaneous::auto_strafe(CUserCmd* cmd)
{
	if (sdk::local_pawn->m_fFlags() & FL_ONGROUND)
		return;

	// check if player is in noclip or on ladder
	if (const int32_t movetype = sdk::local_pawn->m_MoveType(); movetype == MOVETYPE_NOCLIP || movetype == MOVETYPE_LADDER)
		return;

	// dont do it holding wasd since it's mouse only
	if ((cmd->nButtons.nValue & IN_SPEED) || (cmd->nButtons.nValue & IN_BACK) || (cmd->nButtons.nValue & IN_FORWARD) ||
		(cmd->nButtons.nValue & IN_LEFT) || (cmd->nButtons.nValue & IN_RIGHT))
		return;

	cmd->pBaseCmd->flSideMove = cmd->pBaseCmd->nMousedX > 0 ? -1.0f : 1.0f;
}

void miscellaneous::validate_user_cmd(CUserCmd* cmd, CBaseUserCmdPB* pUserCmd, CCSGOInputHistoryEntryPB* pInputEntry)
{
	if (!pUserCmd) return;

	if (settings.anti_untrusted) {
		if (pInputEntry->pViewCmd->angValue.IsValid())
		{
			pInputEntry->pViewCmd->angValue.Clamp();
			pInputEntry->pViewCmd->angValue.z = 0.f;
		}
		else pInputEntry->pViewCmd->angValue = {};
	}

	// fix movement
	correct_movement(pUserCmd, pInputEntry, angCorrectionView);

	// correct movement buttons while player move have different to buttons values
	// clear all of the move buttons states
	cmd->nButtons.nValue &= (~IN_FORWARD | ~IN_BACK | ~IN_LEFT | ~IN_RIGHT);

	// re-store buttons by active forward/side moves
	if (pUserCmd->flForwardMove > 0.0f)
		cmd->nButtons.nValue |= IN_FORWARD;
	else if (pUserCmd->flForwardMove < 0.0f)
		cmd->nButtons.nValue |= IN_BACK;

	if (pUserCmd->flSideMove > 0.0f)
		cmd->nButtons.nValue |= IN_RIGHT;
	else if (pUserCmd->flSideMove < 0.0f)
		cmd->nButtons.nValue |= IN_LEFT;
}

void miscellaneous::correct_movement(CBaseUserCmdPB* pUserCmd, CCSGOInputHistoryEntryPB* pInputEntry, const QAngle_t& angDesiredViewPoint)
{
	if (pUserCmd == nullptr)
		return;

	Vector_t vecForward = {}, vecRight = {}, vecUp = {};
	angDesiredViewPoint.ToDirections(&vecForward, &vecRight, &vecUp);

	// we don't attempt on forward/right roll, and on up pitch/yaw
	vecForward.z = vecRight.z = vecUp.x = vecUp.y = 0.0f;

	vecForward.NormalizeInPlace();
	vecRight.NormalizeInPlace();
	vecUp.NormalizeInPlace();

	Vector_t vecOldForward = {}, vecOldRight = {}, vecOldUp = {};
	pInputEntry->pViewCmd->angValue.ToDirections(&vecOldForward, &vecOldRight, &vecOldUp);

	// we don't attempt on forward/right roll, and on up pitch/yaw
	vecOldForward.z = vecOldRight.z = vecOldUp.x = vecOldUp.y = 0.0f;

	vecOldForward.NormalizeInPlace();
	vecOldRight.NormalizeInPlace();
	vecOldUp.NormalizeInPlace();

	const float flPitchForward = vecForward.x * pUserCmd->flForwardMove;
	const float flYawForward = vecForward.y * pUserCmd->flForwardMove;
	const float flPitchSide = vecRight.x * pUserCmd->flSideMove;
	const float flYawSide = vecRight.y * pUserCmd->flSideMove;
	const float flRollUp = vecUp.z * pUserCmd->flUpMove;

	// solve corrected movement speed
	pUserCmd->flForwardMove = vecOldForward.x * flPitchSide + vecOldForward.y * flYawSide + vecOldForward.x * flPitchForward + vecOldForward.y * flYawForward + vecOldForward.z * flRollUp;
	pUserCmd->flSideMove = vecOldRight.x * flPitchSide + vecOldRight.y * flYawSide + vecOldRight.x * flPitchForward + vecOldRight.y * flYawForward + vecOldRight.z * flRollUp;
	pUserCmd->flUpMove = vecOldUp.x * flYawSide + vecOldUp.y * flPitchSide + vecOldUp.x * flYawForward + vecOldUp.y * flPitchForward + vecOldUp.z * flRollUp;
}

void miscellaneous::skin_changer()
{
	if (!sdk::local_controller || !sdk::local_pawn || !settings.skin_changer)
		return;

	//using item_t = C_EconItemView * (__fastcall*)(C_BasePlayerWeapon*);
	//static auto sig = signature::find("client.dll", "E8 ? ? ? ? 45 33 ED 48 8D 8D ? ? ? ?").add(0x1).rel32().get<item_t>();

	CPlayer_WeaponServices* pWeaponServices = sdk::local_pawn->m_pWeaponServices();
	if (!pWeaponServices)
		return;

	C_BasePlayerWeapon* pActiveWeapon = pWeaponServices->m_hActiveWeapon().Get<C_BasePlayerWeapon>();
	if (!pActiveWeapon)
		return;

	CCSPlayer_ViewModelServices* pViewModelServices = reinterpret_cast<C_CSPlayerPawnBase*>(sdk::local_pawn)->m_pViewModelServices();
	if (!pViewModelServices) return;

	C_BaseViewModel* pViewModel = pViewModelServices->m_hViewModel()[0].Get<C_BaseViewModel>();
	if (!pViewModel) return;

	CGameSceneNode* pViewModelSceneNode = pViewModel->m_pGameSceneNode();
	if (!pViewModelSceneNode) return;

	C_AttributeContainer pAttributeContainer = pActiveWeapon->m_AttributeManager();
	C_EconItemView pEconItemView = pAttributeContainer.m_Item();

	CGameSceneNode* pWeaponSceneNode = pActiveWeapon->m_pGameSceneNode();
	if (!pWeaponSceneNode)
		return;

	pActiveWeapon->m_nFallbackPaintKit() = settings.skin_id;

	pActiveWeapon->m_nFallbackSeed() = 0;
	pActiveWeapon->m_nFallbackStatTrak() = 1337;
	pActiveWeapon->m_flFallbackWear() = 0.00001f;

	pWeaponSceneNode->SetMeshGroupMask(2);

	pEconItemView.m_iItemIDHigh() = -1;

	if (pViewModel->m_hWeapon() == pActiveWeapon->GetRefEHandle())
		pViewModelSceneNode->SetMeshGroupMask(2);
}
