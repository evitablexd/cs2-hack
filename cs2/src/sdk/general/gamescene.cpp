#include "../../includes.h"

BitFlag CModel::GetBoneFlags(std::int32_t index)
{
    using Fn = std::int32_t(__fastcall*)(CModel*, std::int32_t);

    static auto pattern = signature::find("client.dll", "85 D2 78 16 3B 91").get<Fn>();
    if (!pattern) return {};

    return BitFlag(pattern(this, index));
}

std::int32_t CModel::GetBoneParent(std::int32_t index)
{
    using Fn = std::int32_t(__fastcall*)(CModel*, std::int32_t);

    static auto pattern = signature::find("client.dll", "85 D2 78 17 3B 91 70").get<Fn>();
    if (!pattern) return {};

    return pattern(this, index);
}

void CGameSceneNode::SetMeshGroupMask(uint64_t meshGroupMask)
{
    typedef void(__fastcall* fn)(void*, uint64_t);
    static auto sig = signature::find("client.dll", "E8 ? ? ? ? 48 8B 5C 24 ? 4C 8B 7C 24 ? 4C 8B 74 24 ?").add(0x1).rel32().get<fn>();

    if (!sig) return;

    sig(this, meshGroupMask);
}
