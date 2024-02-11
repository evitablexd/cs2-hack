#include "../../includes.h"

CCSInventoryManager* CCSInventoryManager::GetInstance()
{
    typedef CCSInventoryManager* (__fastcall* fn)();
    static auto sig = signature::find("client.dll", "E8 ? ? ? ? 48 63 BB ? ? ? ? 48 8D 68 28 83 FF FF").add(0x1).rel32().get<fn>();

    if (!sig) return nullptr;

    return sig();
}

CCSPlayerInventory* CCSPlayerInventory::GetInstance()
{
    CCSInventoryManager* pInventoryManager = CCSInventoryManager::GetInstance();
    if (!pInventoryManager) return nullptr;

    return pInventoryManager->GetLocalInventory();
}

bool CCSPlayerInventory::AddEconItem(CEconItem* pItem)
{
    // Helper function to aid in adding items.
    if (!pItem) return false;

    return true;
}

void CCSPlayerInventory::RemoveEconItem(CEconItem* pItem)
{
    // Helper function to aid in removing items.
    if (!pItem) return;

}
