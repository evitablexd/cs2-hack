#include "../../includes.h"

CEconItem* C_EconItemView::GetSOCData()
{
    return nullptr;
}

char* C_EconItemView::GetName()
{
    typedef char*(__fastcall* fn)(void*);
    static auto func = signature::find("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC 20 80 B9 ? ? ? ? ? 48 8B F9 0F 85 ? ? ? ? 33 DB 48 39 99 ? ? ? ? 74 0C 48 8D B1 ? ? ? ? E9 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 84 C0 75 3F E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 8B D3 48 8D B7 ? ? ? ? 83 78 18 01 48 8B 48 10 0F 44 D1 3B 16 75 1D E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 48 8B E8 EB 2A 48 8D B7 ? ? ? ? E8 ? ? ? ? 8B 16 48 8B 08 4C 8B 81 ? ? ? ? 48 8B C8 41 FF D0 48 8B E8 48 85 C0 0F 84 ? ? ? ? 48 8B 07 48 8B CF FF 50 78 48 8B D0 48 8B CD E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 38 9F ? ? ? ? 0F 85 ? ? ? ? 48 8B 87 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 84 C0 75 4B E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 8B D3 48 8D B7 ? ? ? ? 83 78 18 01 48 8B 48 10 0F 44 D1 3B 16 75 22 E8 ? ? ? ? 48 8B C8 48 8B 10 FF 92 ? ? ? ? 48 8B 17 48 8B CF 48 8B D8 FF 52 78 48 8B CB EB 2B E8 ? ? ? ? 8B 16 48 8B 08 4C 8B 81 ? ? ? ? 48 8B C8 41 FF D0 48 8B F0 48 85 C0 74 17 48 8B 17 48 8B CF FF 52 78 48 8B CE 48 8B D0 E8 ? ? ? ? 48 8B D8 48 8B 43 08 48 8D 4B 08 FF 90 ? ? ? ? EB 02").get<fn>();

    return func(this);
}

CCStrike15ItemDefinition* C_EconItemView::GetStaticData()
{
    //return CALL_VIRTUAL(CCStrike15ItemDefinition*, 13, this);
    typedef CCStrike15ItemDefinition* (__fastcall* fn)(void*);
    static auto func = signature::find("client.dll", "E8 ? ? ? ? 48 8B D8 48 8D 4D D0").add(0x1).rel32().get<fn>();

    return func(this);
}

CEconItem* CEconItem::CreateInstance()
{
    return nullptr;
}
