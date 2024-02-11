#pragma once

namespace miscellaneous
{
	void run(CUserCmd* cmd);
	void bunny_hop(CUserCmd* cmd);
	void auto_strafe(CUserCmd* cmd);

	void validate_user_cmd(CUserCmd* cmd, CBaseUserCmdPB* pUserCmd, CCSGOInputHistoryEntryPB* pInputEntry);
	void correct_movement(CBaseUserCmdPB* pUserCmd, CCSGOInputHistoryEntryPB* pInputEntry, const QAngle_t& angDesiredViewPoint);

	void skin_changer();
}