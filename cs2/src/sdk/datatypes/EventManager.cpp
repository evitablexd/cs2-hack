#include "../../includes.h"

void* events_t::get_player_pawn_from_id(std::string_view event_name) noexcept
{
    static const auto fn = signature::find("client.dll", "48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 30 48 8B 01 48 8B F1 41 B0 01 48 8D 4C 24 20 48 8B FA 48 8B 98")
        .get<void* (__fastcall*)(void*, const char*, std::int64_t)>();

    return fn(this, event_name.data(), 0);
}

std::int64_t events_t::get_int(std::string_view event_name) noexcept
{
    static const auto fn = signature::find("client.dll", "48 89 5C 24 08 48 89 74 24 10 48 89 7C 24 18 41 56 48 83 EC 30 48 8B 01 41 8B F0 4C 8B F1 41 B0 01 48 8D 4C 24 20 48 8B DA 48 8B 78")
        .get<std::int64_t(__fastcall*)(void*, const char*) >();

    return fn(this, event_name.data());
}

void* events_t::get_pointer_from_id(const std::string_view event_name) noexcept
{
    // used: "userid", "attackerid"
    std::int64_t id{ };
    {
        static const auto fn = signature::find("client.dll", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 30 48 8B 01 49")
            .get< std::int64_t(__fastcall*)(void*, std::int64_t*, const char*)>();

        fn(this, &id, event_name.data());
    }

    if (id == -1)
        return { };

    // xref: "player_disconnect"
    // client.dll; E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B D8 48 85 C9
    static const auto fn = signature::find("client.dll", "E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 8B D8 48 85 C9").add(0x1).rel32()
        .get<void* (__fastcall*)(std::int64_t) >();

    return fn(id);
}
