#pragma once

class CCSPlayerInventory;

class CCSInventoryManager {
public:
    static CCSInventoryManager* GetInstance();

    auto EquipItemInLoadout(int iTeam, int iSlot, uint64_t iItemID) {
        return CALL_VIRTUAL(bool, 52, this, iTeam, iSlot, iItemID);
    }

    auto GetLocalInventory() {
        return CALL_VIRTUAL(CCSPlayerInventory*, 55, this);
    }
};

class CEconItem;
class C_EconItemView;
class CCSPlayerInventory
{
public:
    static CCSPlayerInventory* GetInstance();

    auto GetItemInLoadout(int iClass, int iSlot) {
        return CALL_VIRTUAL(C_EconItemView*, 8, this, iClass, iSlot);
    }

    bool AddEconItem(CEconItem* pItem);
    void RemoveEconItem(CEconItem* pItem);

    auto& GetItemVector() {
        return *reinterpret_cast<CUtlVector<C_EconItemView*>*>(
            (uintptr_t)(this) + 0x20);
    }
};