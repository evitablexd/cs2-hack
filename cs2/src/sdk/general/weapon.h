#pragma once

class CEconItem
{
private:
	void SetDynamicAttributeValue(int index, void* value);
	void SetDynamicAttributeValueString(int index, const char* value);

public:
	static CEconItem* CreateInstance();

	auto Destruct() { return CALL_VIRTUAL(void, 1, this, true); }

	void SetPaintKit(float kit) { SetDynamicAttributeValue(6, &kit); }
	void SetPaintSeed(float seed) { SetDynamicAttributeValue(7, &seed); }
	void SetPaintWear(float wear) { SetDynamicAttributeValue(8, &wear); }
	void SetStatTrak(int count) { SetDynamicAttributeValue(80, &count); }
	void SetStatTrakType(int type) { SetDynamicAttributeValue(81, &type); }
	void SetCustomName(const char* pName) {
		SetDynamicAttributeValueString(111, pName);
	}

	char pad0[0x10];  // 2 vtables
	uint64_t m_ulID;
	uint64_t m_ulOriginalID;
	void* m_pCustomDataOptimizedObject;
	uint32_t m_unAccountID;
	uint32_t m_unInventory;
	uint16_t m_unDefIndex;
	uint16_t m_unOrigin : 5;
	uint16_t m_nQuality : 4;
	uint16_t m_unLevel : 2;
	uint16_t m_nRarity : 4;
	uint16_t m_dirtybitInUse : 1;
	int16_t m_iItemSet;
	int m_bSOUpdateFrame;
	uint8_t m_unFlags;
};

class CCStrike15ItemDefinition {
public:
	PAD(0x10); // 0x0
	std::uint16_t m_nWeaponId; // 0x10
	PAD(0x5A); // 0x12
	const char* m_szItemLocalizedName; // 0x70
	PAD(0x8); // 0x78
	const char* m_szItemType; // 0x80
	PAD(0x8); // 0x88
	const char* m_szItemDescription; // 0x90
	PAD(0x10); // 0x98
	const char* m_szInventoryImage; // 0xA8
	PAD(0x28); // 0xB0
	const char* m_szPlayerModel; // 0xD8
	PAD(0x8); // 0xE0
	const char* m_szWorldModel; // 0xE8
	PAD(0x10); // 0xF0
	int m_nStickerCount; // 0x100;
	PAD(0xE0); // 0xF0
	const char* m_szPaintableMaterialName; // 0x1E0
	PAD(0x10); // 0x1E8
	const char* m_szItemName; // 0x1F8
	PAD(0x180); // 0x200
};
class C_EconItemView
{
public:
	CEconItem* GetSOCData();

	auto GetCustomPaintKitIndex() { return CALL_VIRTUAL(int, 2, this); }

	char* GetName();

	CCStrike15ItemDefinition* GetStaticData();

	SCHEMA_FIELD(m_iItemDefinitionIndex, "C_EconItemView", "m_iItemDefinitionIndex", uint16_t);
	SCHEMA_FIELD(m_iItemID, "C_EconItemView", "m_iItemID", uint64_t);
	SCHEMA_FIELD(m_iItemIDLow, "C_EconItemView", "m_iItemIDLow", uint32_t);
	SCHEMA_FIELD(m_iItemIDHigh, "C_EconItemView", "m_iItemIDHigh", uint32_t);
	SCHEMA_FIELD(m_iAccountID, "C_EconItemView", "m_iAccountID", uint32_t);
};

class C_AttributeContainer {
public:
	SCHEMA_FIELD(m_Item, "C_AttributeContainer", "m_Item", C_EconItemView);
};

class C_EconEntity : public C_BaseModelEntity
{
public:
	SCHEMA_FIELD(m_OriginalOwnerXuidLow, "C_EconEntity", "m_OriginalOwnerXuidLow", uint32_t);
	SCHEMA_FIELD(m_OriginalOwnerXuidHigh, "C_EconEntity", "m_OriginalOwnerXuidHigh", uint32_t);
	SCHEMA_FIELD(m_nFallbackPaintKit, "C_EconEntity", "m_nFallbackPaintKit", int32_t);
	SCHEMA_FIELD(m_nFallbackSeed, "C_EconEntity", "m_nFallbackSeed", int32_t);
	SCHEMA_FIELD(m_flFallbackWear, "C_EconEntity", "m_flFallbackWear", float);
	SCHEMA_FIELD(m_nFallbackStatTrak, "C_EconEntity", "m_nFallbackStatTrak", int32_t);
	SCHEMA_FIELD(m_AttributeManager, "C_EconEntity", "m_AttributeManager", C_AttributeContainer);
};

class C_BasePlayerWeapon : public C_EconEntity
{
public:
	SCHEMA_FIELD(m_iClip1, "C_BasePlayerWeapon", "m_iClip1", int32_t);
	SCHEMA_FIELD(m_iClip2, "C_BasePlayerWeapon", "m_iClip2", int32_t);
	SCHEMA_FIELD(m_pReserveAmmo, "C_BasePlayerWeapon", "m_pReserveAmmo", int32_t[2]);
	SCHEMA_FIELD(m_nNextSecondaryAttackTick, "C_BasePlayerWeapon", "m_nNextSecondaryAttackTick", int); // GameTick_t
	SCHEMA_FIELD(m_flNextPrimaryAttackTickRatio, "C_BasePlayerWeapon", "m_flNextPrimaryAttackTickRatio", float);
	SCHEMA_FIELD(m_nNextPrimaryAttackTick, "C_BasePlayerWeapon", "m_nNextPrimaryAttackTick", int); // GameTick_t
};

class CCSWeaponBaseVData;
class CBasePlayerWeaponVData;

class C_CSWeaponBase : public C_BasePlayerWeapon
{
public:
	SCHEMA_FIELD(m_flFireSequenceStartTime, "C_CSWeaponBase", "m_flFireSequenceStartTime", float);
	SCHEMA_FIELD(m_flGunAccuracyPosition, "C_CSWeaponBase", "m_flGunAccuracyPosition", float);
	SCHEMA_FIELD(m_flTurningInaccuracyDelta, "C_CSWeaponBase", "m_flTurningInaccuracyDelta", float);
	SCHEMA_FIELD(m_fAccuracyPenalty, "C_CSWeaponBase", "m_fAccuracyPenalty", float);
	SCHEMA_FIELD(m_iRecoilIndex, "C_CSWeaponBase", "m_iRecoilIndex", int32_t);
	SCHEMA_FIELD(m_flRecoilIndex, "C_CSWeaponBase", "m_flRecoilIndex", float);
	SCHEMA_FIELD(m_bBurstMode, "C_CSWeaponBase", "m_bBurstMode", bool);
	SCHEMA_FIELD(m_flPostponeFireReadyFrac, "C_CSWeaponBase", "m_flPostponeFireReadyFrac", float);
	SCHEMA_FIELD(m_nPostponeFireReadyTicks, "C_CSWeaponBase", "m_nPostponeFireReadyTicks", int); // GameTick_t
	SCHEMA_FIELD(m_bInReload, "C_CSWeaponBase", "m_bInReload", bool);
	SCHEMA_FIELD(m_bReloadVisuallyComplete, "C_CSWeaponBase", "m_bReloadVisuallyComplete", bool);
	SCHEMA_FIELD(m_bSilencerOn, "C_CSWeaponBase", "m_bSilencerOn", bool);
	SCHEMA_FIELD(m_bWasOwnedByCT, "C_CSWeaponBase", "m_bWasOwnedByCT", bool);
	SCHEMA_FIELD(m_bWasOwnedByTerrorist, "C_CSWeaponBase", "m_bWasOwnedByTerrorist", bool);
	SCHEMA_FIELD(m_fLastShotTime, "C_CSWeaponBase", "m_fLastShotTime", float); // GameTime_t

	CCSWeaponBaseVData* GetWeaponVData()
	{
		return static_cast<CCSWeaponBaseVData*>(GetVData());
	}
};

class C_CSWeaponBaseGun : public C_CSWeaponBase
{
public:
	SCHEMA_FIELD(m_zoomLevel, "C_CSWeaponBaseGun", "m_zoomLevel", int32_t);
};

class CBasePlayerWeaponVData
{
public:
	SCHEMA_FIELD(m_bAllowFlipping, "CBasePlayerWeaponVData", "m_bAllowFlipping", bool);
	SCHEMA_FIELD(m_bBuiltRightHanded, "CBasePlayerWeaponVData", "m_bBuiltRightHanded", bool);
	SCHEMA_FIELD(m_iMaxClip1, "CBasePlayerWeaponVData", "m_iMaxClip1", int32_t);
	SCHEMA_FIELD(m_iMaxClip2, "CBasePlayerWeaponVData", "m_iMaxClip2", int32_t);
	SCHEMA_FIELD(m_iDefaultClip1, "CBasePlayerWeaponVData", "m_iDefaultClip1", int32_t);
	SCHEMA_FIELD(m_iDefaultClip2, "CBasePlayerWeaponVData", "m_iDefaultClip2", int32_t);
	SCHEMA_FIELD(m_iWeight, "CBasePlayerWeaponVData", "m_iWeight", int32_t);
	SCHEMA_FIELD(m_iSlot, "CBasePlayerWeaponVData", "m_iSlot", int32_t);
};

class CCSWeaponBaseVData : public CBasePlayerWeaponVData
{
public:
	SCHEMA_FIELD(m_bMeleeWeapon, "CCSWeaponBaseVData", "m_bMeleeWeapon", bool);
	SCHEMA_FIELD(m_bHasBurstMode, "CCSWeaponBaseVData", "m_bHasBurstMode", bool);
	SCHEMA_FIELD(m_bIsRevolver, "CCSWeaponBaseVData", "m_bIsRevolver", bool);
	SCHEMA_FIELD(m_bCannotShootUnderwater, "CCSWeaponBaseVData", "m_bCannotShootUnderwater", bool);
	SCHEMA_FIELD(m_bIsFullAuto, "CCSWeaponBaseVData", "m_bIsFullAuto", bool);
	SCHEMA_FIELD(m_bUnzoomsAfterShot, "CCSWeaponBaseVData", "m_bUnzoomsAfterShot", bool);
	SCHEMA_FIELD(m_bHideViewModelWhenZoomed, "CCSWeaponBaseVData", "m_bHideViewModelWhenZoomed", bool);
	SCHEMA_FIELD(m_nNumBullets, "CCSWeaponBaseVData", "m_nNumBullets", int32_t);
	SCHEMA_FIELD(m_nRecoilSeed, "CCSWeaponBaseVData", "m_nRecoilSeed", int32_t);
	SCHEMA_FIELD(m_nZoomLevels, "CCSWeaponBaseVData", "m_nZoomLevels", int32_t);
	SCHEMA_FIELD(m_nZoomFOV1, "CCSWeaponBaseVData", "m_nZoomFOV1", int32_t);
	SCHEMA_FIELD(m_nZoomFOV2, "CCSWeaponBaseVData", " m_nZoomFOV2", int32_t);
	SCHEMA_FIELD(m_nDamage, "CCSWeaponBaseVData", " m_nDamage", int32_t);
	SCHEMA_FIELD(m_nSpreadSeed, "CCSWeaponBaseVData", "m_nSpreadSeed", int32_t);
	SCHEMA_FIELD(m_flInaccuracyPitchShift, "CCSWeaponBaseVData", "m_flInaccuracyPitchShift", float);
	SCHEMA_FIELD(m_flInaccuracyAltSoundThreshold, "CCSWeaponBaseVData", "m_flInaccuracyAltSoundThreshold", float);
	SCHEMA_FIELD(m_flBotAudibleRange, "CCSWeaponBaseVData", "m_flBotAudibleRange", float);
	SCHEMA_FIELD(m_flHeadshotMultiplier, "CCSWeaponBaseVData", "m_flHeadshotMultiplier", float);
	SCHEMA_FIELD(m_flArmorRatio, "CCSWeaponBaseVData", "m_flArmorRatio", float);
	SCHEMA_FIELD(m_flPenetration, "CCSWeaponBaseVData", "m_flPenetration", float);
	SCHEMA_FIELD(m_flRange, "CCSWeaponBaseVData", "m_flRange", float);
	SCHEMA_FIELD(m_flRangeModifier, "CCSWeaponBaseVData", "m_flRangeModifier", float);
	SCHEMA_FIELD(m_flMaxSpeed, "CCSWeaponBaseVData", "m_flMaxSpeed", float);
	SCHEMA_FIELD(m_flSpread, "CCSWeaponBaseVData", "m_flSpread", float);
	SCHEMA_FIELD(m_flInaccuracyCrouch, "CCSWeaponBaseVData", "m_flInaccuracyCrouch", float);
	SCHEMA_FIELD(m_flInaccuracyStand, "CCSWeaponBaseVData", "m_flInaccuracyStand", float);
	SCHEMA_FIELD(m_flInaccuracyMove, "CCSWeaponBaseVData", "m_flInaccuracyMove", float);
};