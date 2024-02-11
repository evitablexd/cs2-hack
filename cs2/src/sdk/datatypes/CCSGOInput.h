#pragma once

#define MULTIPLAYER_BACKUP 150

class CCSGOInput
{
public:
	PAD(0x250);
	CUserCmd arrCommands[MULTIPLAYER_BACKUP];
	PAD(0x1);
	bool bInThirdPerson;
	PAD(0x22)
	std::int32_t nSequenceNumber;
	std::int32_t nOldSequenceNumber;
	PAD(0x4);
	double dbUnknown; // look's like a timer
	std::uint64_t nButtonState1;
	std::uint64_t nButtonState2;
	std::uint64_t nButtonState3;
	std::uint64_t nButtonState4; // ?
	PAD(0xC);
	std::int32_t nMouseDeltaX;
	std::int32_t nMouseDeltaY;
	PAD(0xC);
	std::int64_t nOldPressedButton;
	bool bIsButtonPressed;
	PAD(0x10F);
	QAngle_t angViewAngles;
	PAD(0x8C);
	bool bUnknownBool;
	PAD(0x7);

	CUserCmd* GetUserCmd()
	{
		return &arrCommands[nSequenceNumber % MULTIPLAYER_BACKUP];
	}

	void SetViewAngle(QAngle_t& angView)
	{
		// @ida: this got called before GetMatricesForView
		typedef int64_t(__fastcall* fn)(void*, std::int32_t, QAngle_t&);
		static auto oSetViewAngle = signature::find("client.dll", "F2 41 0F 10 00 4C 63 CA").get<fn>();

		oSetViewAngle(this, 0, std::ref(angView));
	}
};