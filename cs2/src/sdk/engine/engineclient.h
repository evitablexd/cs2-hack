#pragma once

enum EClientFrameStage : int
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	// a network packet is being received
	FRAME_NET_UPDATE_START,
	// data has been received and we are going to start calling postdataupdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// data has been received and called postdataupdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// received all packets, we can now do interpolation, prediction, etc
	FRAME_NET_UPDATE_END,
	// start rendering the scene
	FRAME_RENDER_START,
	// finished rendering the scene
	FRAME_RENDER_END,
	FRAME_NET_FULL_FRAME_UPDATE_ON_REMOVE
};

class CEngineClient
{
public:

	__forceinline int GetMaxClients() {
		return CALL_VIRTUAL(int, 31, this);
	}

	__forceinline bool IsInGame() { return CALL_VIRTUAL(bool, 32, this); }
	__forceinline bool IsConnected() { return CALL_VIRTUAL(bool, 33, this); }

	__forceinline Vector2D_t GetScreenSize() {
		std::int32_t w, h;

		CALL_VIRTUAL(void, 49, this, &w, &h);

		return { static_cast<float>(w), static_cast<float>(h) };
	}

	__forceinline int GetLocalPlayer()
	{
		int nIndex = -1;

		CALL_VIRTUAL(void, 44, this, std::ref(nIndex));

		return nIndex + 1;
	}

	const char* GetProductVersionString()
	{
		return CALL_VIRTUAL(const char*, 77, this);
	}
};

class CLocalize {
public:
	char* FindSafe(const char* tokenName);
};