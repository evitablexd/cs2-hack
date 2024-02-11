#pragma once

struct Ray_t
{
	Vector_t vecStart;
	Vector_t vecEnd;
	Vector_t vecMins;
	Vector_t vecMaxs;
	PAD(0x4);
	std::uint8_t UnkType;
};

struct SurfaceData_t
{
	PAD(0x8);
	float m_flPenetrationModifier;
	float m_flDamageModifier;
	PAD(0x4);
	int m_iMaterial;
};

struct TraceHitboxData_t
{
	PAD(0x38);
	int m_nHitGroup;
	PAD(0x4);
	int m_nHitboxId;
};

class C_CSPlayerPawn;
struct GameTrace_t
{
	GameTrace_t() = default;
	SurfaceData_t* GetSurfaceData();

	int GetHitboxId();
	int GetHitgroup();
	bool DidHitWorld() const;

	void* m_pSurface;
	C_CSPlayerPawn* m_pHitEntity;
	TraceHitboxData_t* m_pHitboxData;
	PAD(0x38);
	std::uint32_t m_uContents;
	PAD(0x24);
	Vector_t m_vecStartPos;
	Vector_t m_vecEndPos;
	Vector_t m_vecNormal;
	Vector_t m_vecPosition;
	PAD(0x4);
	float m_flFraction;
	PAD(0x6);
	bool m_bAllSolid;
	PAD(0x4D)
}; // Size: 0xB8

struct TraceFilter_t
{
	PAD(0x8);
	std::int64_t m_uTraceMask;
	std::array< std::int64_t, 2 > m_v1;
	std::array< std::int32_t, 4 > m_arrSkipHandles;
	std::array< std::int16_t, 2 > m_arrCollisions;
	std::int16_t m_v2;
	std::uint8_t m_v3;
	std::uint8_t m_v4;
	std::uint8_t m_v5;

	TraceFilter_t() = default;
	TraceFilter_t(std::uint32_t uMask, C_CSPlayerPawn* pSkip1, C_CSPlayerPawn* pSkip2, int nLayer);
};

class CGameTraceManager
{
public:
	__forceinline bool TraceShape(Ray_t* pRay, Vector_t vecStart, Vector_t vecEnd, TraceFilter_t* pFilter, GameTrace_t* pGameTrace)
	{
		using fnTraceShape = bool(__fastcall*)(void*, void*, const Vector_t&, const Vector_t&, void*, void*);
		static auto oTraceShape = signature::find("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 48 89 4C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ?").get<fnTraceShape>();

		return oTraceShape(this, pRay, vecStart, vecEnd, pFilter, pGameTrace);
	}

	__forceinline void ClipTraceToPlayers(Vector_t vecStart, Vector_t vecEnd, TraceFilter_t* pFilter, GameTrace_t* pGameTrace, float flUnknown, float flUnknown2, float flUnknown3)
	{
		using fnClipTraceToPlayers = void(__fastcall*)(const Vector_t&, const Vector_t&, TraceFilter_t*, GameTrace_t*, float, float, float);
		static auto oClipTraceToPlayers = signature::find("client.dll", "48 8B C4 55 56 48 8D ? ? FF FF FF 48 81 EC").get<fnClipTraceToPlayers>();

		oClipTraceToPlayers(vecStart, vecEnd, pFilter, pGameTrace, flUnknown, flUnknown2, flUnknown3);
	}
};