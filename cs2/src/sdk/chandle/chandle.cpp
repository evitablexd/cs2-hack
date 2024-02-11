#include "../../includes.h"

C_BaseEntity* CBaseHandle::GetBaseEntity() const
{
	CGameEntitySystem* pEntitySystem = CGameEntitySystem::GetInstance();
	if (!pEntitySystem) {  return nullptr; }

	return pEntitySystem->Get(GetEntryIndex());
}
