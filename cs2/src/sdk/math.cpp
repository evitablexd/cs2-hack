#include "../includes.h"

bool math::Setup()
{
	bool bSuccess = true;

	const HMODULE hTier0Lib = GetModuleHandleA("tier0.dll");
	if (hTier0Lib == NULL)
		return false;

	fnRandomSeed = reinterpret_cast<decltype(fnRandomSeed)>(GetProcAddress(hTier0Lib, "RandomSeed"));
	bSuccess &= (fnRandomSeed != nullptr);

	fnRandomFloat = reinterpret_cast<decltype(fnRandomFloat)>(GetProcAddress(hTier0Lib, "RandomFloat"));
	bSuccess &= (fnRandomFloat != nullptr);

	fnRandomFloatExp = reinterpret_cast<decltype(fnRandomFloatExp)>(GetProcAddress(hTier0Lib, "RandomFloatExp"));
	bSuccess &= (fnRandomFloatExp != nullptr);

	fnRandomInt = reinterpret_cast<decltype(fnRandomInt)>(GetProcAddress(hTier0Lib, "RandomInt"));
	bSuccess &= (fnRandomInt != nullptr);

	fnRandomGaussianFloat = reinterpret_cast<decltype(fnRandomGaussianFloat)>(GetProcAddress(hTier0Lib, "RandomGaussianFloat"));
	bSuccess &= (fnRandomGaussianFloat != nullptr);

	return bSuccess;
}

void math::AngleVectors(const QAngle_t& angles, Vector_t& forward)
{
	const auto sp = sin(deg_to_rad(angles.x)), cp = cos(deg_to_rad(angles.x)),
		sy = sin(deg_to_rad(angles.y)), cy = cos(deg_to_rad(angles.y));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

float math::GetFOV(const QAngle_t& view_angles, const Vector_t& start, const Vector_t& end)
{
	Vector_t dir, fw;

	// get direction and normalize.
	dir = (end - start).Normalized();

	// get the forward direction vector of the view angles.
	AngleVectors(view_angles, fw);

	// get the angle between the view angles forward directional vector and the target location.
	return std::max(rad_to_deg(std::acos(fw.DotProduct(dir))), 0.f);
}

float math::NormalizeYaw(float angle) {
	if (!std::isfinite(angle))
		angle = 0.f;

	return std::remainderf(angle, 360.0f);
}

void math::NormalizeAngle(float& angle)
{
	float rot;

	// bad number.
	if (!std::isfinite(angle)) {
		angle = 0.f;
		return;
	}

	// no need to normalize this angle.
	if (angle >= -180.f && angle <= 180.f)
		return;

	// get amount of rotations needed.
	rot = std::round(std::abs(angle / 360.f));

	// normalize.
	angle = (angle < 0.f) ? angle + (360.f * rot) : angle - (360.f * rot);
}
