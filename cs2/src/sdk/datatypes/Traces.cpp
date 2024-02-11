#include "../../includes.h"

SurfaceData_t* GameTrace_t::GetSurfaceData()
{
	using fnGetSurfaceData = std::uint64_t(__fastcall*)(void*);
	static auto oGetSurfaceData = signature::find("client.dll", "48 63 41 10 48 8B 0D ? ? ? ? 48 6B C0 1C 48 03 41 28").get<fnGetSurfaceData>();
	return reinterpret_cast<SurfaceData_t*>(oGetSurfaceData(m_pSurface));
}

int GameTrace_t::GetHitboxId()
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitboxId;
	return 0;
}

int GameTrace_t::GetHitgroup()
{
	if (m_pHitboxData)
		return m_pHitboxData->m_nHitGroup;
	return 0;
}

bool GameTrace_t::DidHitWorld() const
{
	C_BaseEntity* pWorldEntity = interfaces::g_game_resouce->entity_system->Get(0);
	if (!pWorldEntity)
		return false;

	return reinterpret_cast<C_BaseEntity*>(m_pHitEntity) == pWorldEntity;
}

TraceFilter_t::TraceFilter_t(std::uint32_t uMask, C_CSPlayerPawn* pSkip1, C_CSPlayerPawn* pSkip2, int nLayer)
{
	m_uTraceMask = uMask;
	m_v1[0] = m_v1[1] = 0;
	m_v2 = 7;
	m_v3 = nLayer;
	m_v4 = 0x49;
	m_v5 = 0;

	m_arrSkipHandles[0] = pSkip1->GetEntityHandle();
	m_arrSkipHandles[1] = pSkip2->GetOwnerHandle();
	m_arrSkipHandles[2] = 0;
	m_arrSkipHandles[3] = 0;

	m_arrCollisions[0] = pSkip1->GetCollisionMask();
	m_arrCollisions[1] = 0;
}