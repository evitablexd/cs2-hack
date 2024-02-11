#include "../../includes.h"

std::uintptr_t key_string_t::find_key(const std::string_view name)
{
    static const auto fn = signature::find("particles.dll", "48 89 5C 24 08 57 48 81 EC C0 00 00 00 33 C0 8B").get< std::uintptr_t(__fastcall*)(const char*, int, int) >();

    return fn(name.data(), 0x12, k_unk_key_hash);
}
