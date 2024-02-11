#pragma once

#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
#define ENT_ENTRY_MASK 0x7FFF
#define NUM_SERIAL_NUM_SHIFT_BITS 15

// @source: https://developer.valvesoftware.com/wiki/Entity_limit#Source_2_limits
#define ENT_MAX_NETWORKED_ENTRY 16384

class C_BaseEntity;

class CBaseHandle
{
public:
	CBaseHandle() noexcept :
		nIndex(INVALID_EHANDLE_INDEX) { }

	void Init(const int nEntry, const int nSerial) noexcept
	{
		assert(nEntry >= 0 && (nEntry & ENT_ENTRY_MASK) == nEntry);
		assert(nSerial >= 0 && nSerial < (1 << NUM_SERIAL_NUM_SHIFT_BITS));

		nIndex = nEntry | (nSerial << NUM_SERIAL_NUM_SHIFT_BITS);
	}

	bool operator!=(const CBaseHandle& other) const noexcept
	{
		return nIndex != other.nIndex;
	}

	bool operator==(const CBaseHandle& other) const noexcept
	{
		return nIndex == other.nIndex;
	}

	bool operator<(const CBaseHandle& other) const noexcept
	{
		return nIndex < other.nIndex;
	}

	[[nodiscard]] bool IsValid() const noexcept
	{
		return nIndex != INVALID_EHANDLE_INDEX;
	}

	[[nodiscard]] int GetEntryIndex() const noexcept
	{
		return static_cast<int>(nIndex & ENT_ENTRY_MASK);
	}

	[[nodiscard]] int GetSerialNumber() const noexcept
	{
		return static_cast<int>(nIndex >> NUM_SERIAL_NUM_SHIFT_BITS);
	}

	C_BaseEntity* GetBaseEntity() const;

	template <typename T = C_BaseEntity>
	T* Get() const {
		return reinterpret_cast<T*>(GetBaseEntity());
	}

private:
	std::uint32_t nIndex;
};