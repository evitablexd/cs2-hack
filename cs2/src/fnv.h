#pragma once

using FNV1A_t = std::uint64_t;

/*
 * 64-BIT FNV1A HASH
 */
namespace FNV1A
{
	/* @section: [internal] constants */
	constexpr FNV1A_t ullBasis = 0xCBF29CE484222325ULL;
	constexpr FNV1A_t ullPrime = 0x100000001B3ULL;

	constexpr uint32_t val_32_const = 0x811c9dc5;
	constexpr uint32_t prime_32_const = 0x1000193;

	/* @section: get */
	/// @param[in] szString string for which you want to generate a hash
	/// @param[in] uKey key of hash generation
	/// @returns: calculated at compile-time hash of given string
	consteval FNV1A_t HashConst(const char* szString, const FNV1A_t uKey = ullBasis) noexcept
	{
		return (szString[0] == '\0') ? uKey : HashConst(&szString[1], (uKey ^ static_cast<FNV1A_t>(szString[0])) * ullPrime);
	}

	/// @param[in] szString string for which you want to generate a hash
	/// @param[in] uKey key of hash generation
	/// @returns: calculated at run-time hash of given string
	inline FNV1A_t Hash(const char* szString, FNV1A_t uKey = ullBasis) noexcept
	{
		const std::size_t nLength = CRT::StringLength(szString);

		for (std::size_t i = 0U; i < nLength; ++i)
		{
			uKey ^= szString[i];
			uKey *= ullPrime;
		}

		return uKey;
	}

	inline constexpr uint32_t hash_32_fnv1a_const(
		const char* const str, const uint32_t value = val_32_const) noexcept {
		return (str[0] == '\0')
			? value
			: hash_32_fnv1a_const(&str[1],
				(value ^ uint32_t(str[0])) * prime_32_const);
	}
}