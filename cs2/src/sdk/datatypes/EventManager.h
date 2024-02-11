#pragma once

struct EventHash_t
{
    uint32_t hash;
    const char* str;
};

constexpr EventHash_t operator""_event(const char* key, size_t len) {
	const uint32_t m = 0x5bd1e995;
	const int r = 24;

	uint32_t h = 0x31415926 ^ (uint32_t)len;

	const unsigned char* data = (const unsigned char*)key;

	while ((uint32_t)len >= 4) {
		uint32_t k = *(uint32_t*)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	switch ((uint32_t)len) {
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return { h, key };
}

class CCSPlayerController;
struct events_t
{
	// xref: client.dll & 4C 8B F0 8B CE ("cs_pre_restart")
	__forceinline const char* get_name() noexcept { return CALL_VIRTUAL(const char*, 1u, this); }

    __forceinline bool get_bool(const std::string_view token_name) noexcept {
        key_string_t token(token_name.data(), -1);
        return CALL_VIRTUAL(bool, 6u, this, &token, false);
    }

    // xref: client.dll & 89 85 08 0B 00 00 ("damage_taken")
    __forceinline int get_int_from_hash(const std::string_view token_name) noexcept {
        key_string_t token(token_name.data(), -1);
        return CALL_VIRTUAL(int, 7u, this, &token, 0);
    }

    // xref: client.dll & 4C 8B A8 80 00 00 00 ("killer")
    __forceinline CCSPlayerController* get_player_controller(const std::string_view token_name) noexcept {
        key_string_t token(token_name.data(), -1);
        return CALL_VIRTUAL(CCSPlayerController*, 16u, this, &token);
    }

    __forceinline float get_float(const std::string_view token_name) noexcept {
        key_string_t token(token_name.data(), -1);
        return CALL_VIRTUAL(float, 9u, this, &token, 0.0f);
    }

    // xref: client.dll & 48 89 54 24 10 48 89 4C 24 08 55 53 56 57 41 55 41 56 41 57 48 8D AC 24 D0 DF FF FF ("weapon")
    __forceinline const char* get_string(const std::string_view token_name) noexcept {
        key_string_t token(token_name.data(), -1);
        return CALL_VIRTUAL(const char*, 10u, this, &token, nullptr);
    }

    void* get_player_pawn_from_id(std::string_view event_name) noexcept;

    std::int64_t get_int(std::string_view event_name) noexcept;
    void* get_pointer_from_id(const std::string_view event_name) noexcept;
};

class EventManager
{
public:
	void init_event(void* player_pawn_base, const std::string_view event_name, void* unk = nullptr) noexcept {
		return CALL_VIRTUAL(void, 3u, this, player_pawn_base, event_name, unk);
	}
};