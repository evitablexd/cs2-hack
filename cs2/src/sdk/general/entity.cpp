#include "../../includes.h"

CBaseHandle CEntityInstance::GetRefEHandle()
{
    CBaseHandle handle{};

    CEntityIdentity* identity = m_pEntity();
    if (!identity) {
        return handle;
    }

    CBaseHandle unkHandle = *(CBaseHandle*)((uintptr_t)identity + 0x10);
    handle.Init(unkHandle.GetEntryIndex(), unkHandle.GetSerialNumber() - (identity->m_flags() & 1));

    return handle;
}
