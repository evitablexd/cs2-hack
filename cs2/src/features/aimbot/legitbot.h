#pragma once

class Aimbot
{
private:
	C_CSPlayerPawn* target_pawn = nullptr;

public:
	void Run(CUserCmd* pCmd, CCSGOInputHistoryEntryPB* pInputEntry);
	C_CSPlayerPawn* GetTarget(CBaseUserCmdPB* pCmd, CCSGOInputHistoryEntryPB* pInputEntry);
};