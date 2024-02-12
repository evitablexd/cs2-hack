#pragma once

enum ELifeState : int
{
    LIFE_ALIVE = 0,
    LIFE_DYING,
    LIFE_DEAD,
    LIFE_RESPAWNABLE,
    LIFE_DISCARDBODY
};

enum EHitgroups {
    HITGROUP_INVALID = -1,
    HITGROUP_GENERIC = 0,
    HITGROUP_HEAD,
    HITGROUP_CHEST,
    HITGROUP_STOMACH,
    HITGROUP_LEFTARM,
    HITGROUP_RIGHTARM,
    HITGROUP_LEFTLEG,
    HITGROUP_RIGHTLEG,
    HITGROUP_NECK,
    HITGROUP_UNUSED,
    HITGROUP_GEAR,
    HITGROUP_SPECIAL,
    HITGROUP_COUNT,
};

enum EFlags : int
{
    FL_ONGROUND = (1 << 0), // entity is at rest / on the ground
    FL_DUCKING = (1 << 1), // player is fully crouched/uncrouched
    FL_ANIMDUCKING = (1 << 2), // player is in the process of crouching or uncrouching but could be in transition
    FL_WATERJUMP = (1 << 3), // player is jumping out of water
    FL_ONTRAIN = (1 << 4), // player is controlling a train, so movement commands should be ignored on client during prediction
    FL_INRAIN = (1 << 5), // entity is standing in rain
    FL_FROZEN = (1 << 6), // player is frozen for 3rd-person camera
    FL_ATCONTROLS = (1 << 7), // player can't move, but keeps key inputs for controlling another entity
    FL_CLIENT = (1 << 8), // entity is a client (player)
    FL_FAKECLIENT = (1 << 9), // entity is a fake client, simulated server side; don't send network messages to them
    FL_INWATER = (1 << 10), // entity is in water
    FL_FLY = (1 << 11),
    FL_SWIM = (1 << 12),
    FL_CONVEYOR = (1 << 13),
    FL_NPC = (1 << 14),
    FL_GODMODE = (1 << 15),
    FL_NOTARGET = (1 << 16),
    FL_AIMTARGET = (1 << 17),
    FL_PARTIALGROUND = (1 << 18), // entity is standing on a place where not all corners are valid
    FL_STATICPROP = (1 << 19), // entity is a static property
    FL_GRAPHED = (1 << 20),
    FL_GRENADE = (1 << 21),
    FL_STEPMOVEMENT = (1 << 22),
    FL_DONTTOUCH = (1 << 23),
    FL_BASEVELOCITY = (1 << 24), // entity have applied base velocity this frame
    FL_WORLDBRUSH = (1 << 25), // entity is not moveable/removeable brush (part of the world, but represented as an entity for transparency or something)
    FL_OBJECT = (1 << 26),
    FL_KILLME = (1 << 27), // entity is marked for death and will be freed by the game
    FL_ONFIRE = (1 << 28),
    FL_DISSOLVING = (1 << 29),
    FL_TRANSRAGDOLL = (1 << 30), // entity is turning into client-side ragdoll
    FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
};

enum EEFlags : int
{
    EFL_KILLME = (1 << 0),
    EFL_DORMANT = (1 << 1),
    EFL_NOCLIP_ACTIVE = (1 << 2),
    EFL_SETTING_UP_BONES = (1 << 3),
    EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4),
    EFL_DIRTY_SHADOWUPDATE = (1 << 5),
    EFL_NOTIFY = (1 << 6),
    EFL_FORCE_CHECK_TRANSMIT = (1 << 7),
    EFL_BOT_FROZEN = (1 << 8),
    EFL_SERVER_ONLY = (1 << 9),
    EFL_NO_AUTO_EDICT_ATTACH = (1 << 10),
    EFL_DIRTY_ABSTRANSFORM = (1 << 11),
    EFL_DIRTY_ABSVELOCITY = (1 << 12),
    EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
    EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
    EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
    EFL_HAS_PLAYER_CHILD = (1 << 16),
    EFL_IN_SKYBOX = (1 << 17),
    EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),
    EFL_TOUCHING_FLUID = (1 << 19),
    EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
    EFL_NO_ROTORWASH_PUSH = (1 << 21),
    EFL_NO_THINK_FUNCTION = (1 << 22),
    EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),
    EFL_CHECK_UNTOUCH = (1 << 24),
    EFL_DONTBLOCKLOS = (1 << 25),
    EFL_DONTWALKON = (1 << 26),
    EFL_NO_DISSOLVE = (1 << 27),
    EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),
    EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),
    EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),
    EFL_NO_DAMAGE_FORCES = (1 << 31)
};

enum EMoveType : int
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY, // no gravity, but still collides with stuff
    MOVETYPE_FLYGRAVITY, // flies through the air and is affected by gravity
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH, // no clip to world, push and crush
    MOVETYPE_NOCLIP, // no gravity, no collisions, still do velocity/absvelocity
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER, // observer movement, depends on player's observer mode
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    MOVETYPE_MAX_BITS = 4
};


class CEntityIdentity
{
public:
	SCHEMA_FIELD(m_designerName, "CEntityIdentity", "m_designerName",
		const char*);

	SCHEMA_FIELD(m_flags, "CEntityIdentity", "m_flags", uint32_t);
};

class CEntityInstance
{
public:
    auto Schema_DynamicBinding() {
        SchemaClassInfoData_t* rv = nullptr;
        CALL_VIRTUAL(void, 36, this, &rv);
        return rv;
    }

	SCHEMA_FIELD(m_pEntity, "CEntityInstance", "m_pEntity", CEntityIdentity*);

    CBaseHandle GetRefEHandle();
};

class C_BaseEntity : public CEntityInstance
{
public:
    __forceinline bool IsBasePlayerController() noexcept {
        return CALL_VIRTUAL(bool, 144, this);
    }

    __forceinline bool IsBasePlayerWeapon() noexcept {
        return CALL_VIRTUAL(bool, 150, this);
    }

    __forceinline  bool IsViewModel() noexcept {
        return CALL_VIRTUAL(bool, 242, this);
    }

    SCHEMA_FIELD(m_pGameSceneNode, "C_BaseEntity", "m_pGameSceneNode",
        CGameSceneNode*);
    SCHEMA_FIELD(m_pCollision, "C_BaseEntity", "m_pCollision",
        CCollisionProperty*);
    SCHEMA_FIELD(m_iTeamNum, "C_BaseEntity", "m_iTeamNum", uint8_t);
    SCHEMA_FIELD(m_hOwnerEntity, "C_BaseEntity", "m_hOwnerEntity", CBaseHandle);
    SCHEMA_FIELD(m_fFlags, "C_BaseEntity", "m_fFlags", uint32_t);
    SCHEMA_FIELD(m_vecVelocity, "C_BaseEntity", "m_vecVelocity", Vector_t);
    SCHEMA_FIELD(m_iHealth, "C_BaseEntity", "m_iHealth", uint32_t);
    SCHEMA_FIELD(m_MoveType, "C_BaseEntity", "m_MoveType", int32_t);
    SCHEMA_FIELD(m_flWaterLevel, "C_BaseEntity", "m_flWaterLevel", float);
    SCHEMA_FIELD_OFFSET(GetVData, "C_BaseEntity", "m_nSubclassID", 0x8, void*);

    __forceinline Vector_t GetInterpolatedOrigin() noexcept {
        if (!this->m_pGameSceneNode()) return Vector_t{ 0, 0, 0 };

        return this->m_pGameSceneNode()->m_vecAbsOrigin();
    }

    __forceinline bool GetBonePosition(const std::int32_t index, Vector_t& position) noexcept {
        CGameSceneNode* gameSceneNode = this->m_pGameSceneNode();
        if (!gameSceneNode) return false;

        Vector4D_t rotation;
        return gameSceneNode->GetBonePositionAndRotation(index, position, rotation);
    }

    __forceinline Vector_t GetOrigin() noexcept {
        if (!this->m_pGameSceneNode()) return Vector_t{ 0, 0, 0 };

        return this->m_pGameSceneNode()->m_vecOrigin();
    }

    __forceinline float DistanceToSquared(C_BaseEntity* entity) noexcept
    {
        const Vector_t& currentOrigin = GetOrigin();
        const Vector_t& entityOrigin = entity->GetOrigin();

        const float _x = (currentOrigin.x - entityOrigin.x);
        const float _y = (currentOrigin.y - entityOrigin.y);
        const float _z = (currentOrigin.z - entityOrigin.z);

        // https://developer.valvesoftware.com/wiki/Dimensions_(Half-Life_2_and_Counter-Strike:_Source)/en
        // 1 foot = 12 units => 1 unit = 0.0254 meters.
        return (_x * _x + _y * _y + _z * _z) * 0.00064516f;
    }
};

class C_BaseGrenade : public C_BaseEntity
{
public:
    SCHEMA_FIELD(m_flDamage, "C_BaseGrenade", "m_flDamage", float);
    SCHEMA_FIELD(m_hThrower, "C_BaseGrenade", "m_hThrower", CBaseHandle);
    SCHEMA_FIELD(m_flDetonateTime, "C_BaseGrenade", "m_flDetonateTime", uint32_t);
    SCHEMA_FIELD(m_bIsLive, "C_BaseGrenade", "m_bIsLive", bool);
};

class C_BaseCSGrenadeProjectile : public C_BaseGrenade
{
public:
    SCHEMA_FIELD(m_vInitialPosition, "C_BaseCSGrenadeProjectile", "m_vInitialPosition", Vector_t);
    SCHEMA_FIELD(m_vInitialVelocity, "C_BaseCSGrenadeProjectile", "m_vInitialVelocity", Vector_t);
    SCHEMA_FIELD(m_nBounces, "C_BaseCSGrenadeProjectile", "m_nBounces", uint32_t);
    SCHEMA_FIELD(m_flSpawnTime, "C_BaseCSGrenadeProjectile", "m_flSpawnTime", uint32_t);
};

class C_SmokeGrenadeProjectile : public C_BaseCSGrenadeProjectile
{
public:
    SCHEMA_FIELD(m_vSmokeColor, "C_SmokeGrenadeProjectile", "m_vSmokeColor", Vector_t);
    SCHEMA_FIELD(bDidSmokeEffect, "C_SmokeGrenadeProjectile", "bDidSmokeEffect", bool);
    SCHEMA_FIELD(m_nSmokeEffectTickBegin, "C_SmokeGrenadeProjectile", "m_nSmokeEffectTickBegin", int32_t);
};

class CGlowProperty
{
public:
    SCHEMA_FIELD(m_iGlowType, "CGlowProperty", "m_iGlowType", int32_t);
    SCHEMA_FIELD(m_iGlowTeam, "CGlowProperty", "m_iGlowTeam", int32_t);
    SCHEMA_FIELD(m_nGlowRangeMin, "CGlowProperty", "m_nGlowRangeMin", int32_t);
    SCHEMA_FIELD(m_glowColorOverride, "CGlowProperty", "m_glowColorOverride", col_t);
    SCHEMA_FIELD(m_bFlashing, "CGlowProperty", "m_bFlashing", bool);
    SCHEMA_FIELD(m_flGlowTime, "CGlowProperty", "m_flGlowTime", float);
    SCHEMA_FIELD(m_flGlowStartTime, "CGlowProperty", "m_flGlowStartTime", float);
    SCHEMA_FIELD(m_bGlowing, "CGlowProperty", "m_bGlowing", bool);
};

class C_PostProcessingVolume
{
public:
    SCHEMA_FIELD(m_flFadeDuration, "C_PostProcessingVolume", "m_flFadeDuration", float);
    SCHEMA_FIELD(m_flMinExposure, "C_PostProcessingVolume", "m_flMinExposure", float);
    SCHEMA_FIELD(m_flMaxExposure, "C_PostProcessingVolume", "m_flMaxExposure", float);
    SCHEMA_FIELD(m_flExposureFadeSpeedUp, "C_PostProcessingVolume", "m_flExposureFadeSpeedUp", float);
    SCHEMA_FIELD(m_flExposureFadeSpeedDown, "C_PostProcessingVolume", "m_flExposureFadeSpeedDown", float);
    SCHEMA_FIELD(m_bExposureControl, "C_PostProcessingVolume", "m_bExposureControl", bool);
    SCHEMA_FIELD(m_bMaster, "C_PostProcessingVolume", "m_bMaster", bool);
};