#pragma once

class CPlayer_WeaponServices
{
public:
	SCHEMA_FIELD(m_hActiveWeapon, "CPlayer_WeaponServices", "m_hActiveWeapon", CBaseHandle);
};

class C_PostProcessingVolume;
class CPlayer_CameraServices
{
public:
	SCHEMA_FIELD(m_vecCsViewPunchAngle, "CPlayer_CameraServices", "m_vecCsViewPunchAngle", QAngle_t);
	SCHEMA_FIELD(m_hActivePostProcessingVolume, "CPlayer_CameraServices", "m_hActivePostProcessingVolume", CBaseHandle);
};


class CPlayer_ObserverServices
{
public:
	SCHEMA_FIELD(m_iObserverMode, "CPlayer_ObserverServices", "m_iObserverMode", uint8_t);
	SCHEMA_FIELD(m_hObserverTarget, "CPlayer_ObserverServices", "m_hObserverTarget", CBaseHandle);
};

class CCSPlayer_ViewModelServices
{
public:
	SCHEMA_FIELD(m_hViewModel, "CCSPlayer_ViewModelServices", "m_hViewModel", CBaseHandle[3]);
};

class CCSPlayer_ItemServices
{
public:
	SCHEMA_FIELD(m_bHasDefuser, "CCSPlayer_ItemServices", "m_bHasDefuser", bool);
	SCHEMA_FIELD(m_bHasHelmet, "CCSPlayer_ItemServices", "m_bHasHelmet", bool);
	SCHEMA_FIELD(m_bHasHeavyArmor, "CCSPlayer_ItemServices", "m_bHasHeavyArmor", bool);
};

class CCSPlayer_PingServices
{
public:
	SCHEMA_FIELD(m_hPlayerPing, "CCSPlayer_PingServices", "m_hPlayerPing", CBaseHandle);
};