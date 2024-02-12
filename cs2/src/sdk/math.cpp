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
