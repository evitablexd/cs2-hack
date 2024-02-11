#pragma once

// convert angle in degrees to radians
#define M_DEG2RAD(DEGREES) ((DEGREES) * (math::_PI / 180.f))
// convert angle in radians to degrees
#define M_RAD2DEG(RADIANS) ((RADIANS) * (180.f / math::_PI))
/// linearly interpolate the value between @a'X0' and @a'X1' by @a'FACTOR'
#define M_LERP(X0, X1, FACTOR) ((X0) + ((X1) - (X0)) * (FACTOR))
/// trigonometry
#define M_COS(ANGLE) cos(ANGLE)
#define M_SIN(ANGLE) sin(ANGLE)
#define M_TAN(ANGLE) tan(ANGLE)
/// power
#define M_POW(BASE, EXPONENT) pow(BASE, EXPONENT)
/// absolute value
#define M_ABS(VALUE) abs(VALUE)
/// square root
#define M_SQRT(VALUE) sqrt(VALUE)
/// floor
#define M_FLOOR(VALUE) floor(VALUE)

namespace math
{
	/* @section: constants */
	// pi value
	inline constexpr float _PI = 3.141592654f;
	// double of pi
	inline constexpr float _2PI = 6.283185307f;
	// half of pi
	inline constexpr float _HPI = 1.570796327f;
	// quarter of pi
	inline constexpr float _QPI = 0.785398163f;
	// reciprocal of double of pi
	inline constexpr float _1DIV2PI = 0.159154943f;
	// golden ratio
	inline constexpr float _PHI = 1.618033988f;

	// capture game's exports
	bool Setup();

	/* @section: algorithm */
	/// alternative of 'std::min'
	/// @returns : minimal value of the given comparable values
	template <typename T>
	[[nodiscard]] __forceinline constexpr const T& Min(const T& left, const T& right) noexcept
	{
		return (right < left) ? right : left;
	}

	/// alternative of 'std::max'
	/// @returns : maximal value of the given comparable values
	template <typename T>
	[[nodiscard]] __forceinline constexpr const T& Max(const T& left, const T& right) noexcept
	{
		return (right > left) ? right : left;
	}

	/// alternative of 'std::clamp'
	/// @returns : value clamped in range ['minimal' .. 'maximal']
	template <typename T>
	[[nodiscard]] __forceinline constexpr const T& Clamp(const T& value, const T& minimal, const T& maximal) noexcept
	{
		return (value < minimal) ? minimal : (value > maximal) ? maximal :
			value;
	}

	/* @section: exponential */
	/// @returns: true if given number is power of two, false otherwise
	template <typename T> requires (std::is_integral_v<T>)
		[[nodiscard]] __forceinline constexpr bool IsPowerOfTwo(const T value) noexcept
	{
		return value != 0 && (value & (value - 1)) == 0;
	}

	// degrees to radians.
	__forceinline constexpr float deg_to_rad(float val) {
		return val * (_PI / 180.f);
	}

	// radians to degrees.
	__forceinline constexpr float rad_to_deg(float val) {
		return val * (180.f / _PI);
	}

	// normalizes an angle.
	void NormalizeAngle(float& angle);

	__forceinline float NormalizedAngle(float angle) {
		NormalizeAngle(angle);
		return angle;
	}

	void AngleVectors(const QAngle_t& angles, Vector_t& forward);
	float GetFOV(const QAngle_t& view_angles, const Vector_t& start, const Vector_t& end);
	float NormalizeYaw(float angle);

	/* @section: random using game's exports */
	inline int(__cdecl* fnRandomSeed)(int iSeed) = nullptr;
	inline float(__cdecl* fnRandomFloat)(float flMinValue, float flMaxValue) = nullptr;
	inline float(__cdecl* fnRandomFloatExp)(float flMinValue, float flMaxValue, float flExponent) = nullptr;
	inline int(__cdecl* fnRandomInt)(int iMinValue, int iMaxValue) = nullptr;
	inline float(__cdecl* fnRandomGaussianFloat)(float flMean, float flStdDev) = nullptr;
}