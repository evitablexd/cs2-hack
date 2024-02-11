#pragma once

class CViewSetup
{
public:
	char pad_0000[1240]; //0x0000
	float fov; //0x04D8
	float viewmodel_fov; //0x04DC
	Vector_t origin; //0x04E0
	char pad_04EC[12]; //0x04EC
	QAngle_t viewangle; //0x04F8
	char pad_0500[7032]; //0x0500
}; //Size: 0x2078

class c_sub_view
{
public:
	char pad_0000[248]; //0x0000
	Vector_t angles; //0x00F8

	Vector_t& get_angles(int i)
	{
		auto fake_result = Vector_t(0, 0, 0);
		auto v6 = 0x108 * i;
		if (v6)
			return *(Vector_t*)(this + v6 - 16);

		return fake_result;
	}
}; //Size: 0x0104
static_assert(sizeof(c_sub_view) == 0x104);