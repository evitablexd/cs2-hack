#pragma once

class CCSPlayerController;
class C_BaseEntity;

class CGameEntitySystem {
public:
    static CGameEntitySystem* GetInstance();

    /// GetClientEntity
    template <typename T = C_BaseEntity>
    T* Get(int nIndex)
    {
        return reinterpret_cast<T*>(this->GetEntityByIndex(nIndex));
    }

    /// GetClientEntityFromHandle
    template <typename T = C_BaseEntity>
    T* Get(CBaseHandle hHandle)
    {
        if (!hHandle.IsValid()) {
            return nullptr;
        }

        return reinterpret_cast<T*>(this->GetEntityByIndex(hHandle.GetEntryIndex()));
    }

    int GetHighestEntityIndex()
    {
        return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + 0x1510);
    }

    CCSPlayerController* GetLocalPlayerController();

private:
    void* GetEntityByIndex(int nIndex);
};

class CGameResourceService
{
public:
    char pad_0000[88]; //0x0000
    CGameEntitySystem* entity_system; //0x0058
    char pad_0060[40]; //0x0060
}; //Size: 0x0088