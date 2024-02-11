#pragma once

class EntitySpottedState_t {
public:
    bool m_bSpotted;                    // 0x8 - 0x9
    unsigned char pad_9[0x3];           // 0x9 - 0xC
    std::uint32_t m_bSpottedByMask[2];  // 0xC - 0x14
};

static_assert(sizeof(EntitySpottedState_t) == 0x14 - 0x8);

class CGlowProperty;
class C_BaseModelEntity : public C_BaseEntity
{
public:
    __forceinline void SetModel(const char* name) noexcept {
        using Fn = void* (__fastcall*)(void*, const char*);

        static auto pattern = signature::find("client.dll", "E8 ? ? ? ? F3 0F 10 4C 3B ?").add(0x1).deref().get<Fn>();
        if (!pattern) return;

        pattern(this, name);
    }

    SCHEMA_FIELD(m_vecViewOffset, "C_BaseModelEntity", "m_vecViewOffset", Vector_t);
    SCHEMA_FIELD(GetGlowProperty, "C_BaseModelEntity", "m_Glow", CGlowProperty);
};

class C_PlantedC4 : public C_BaseModelEntity
{
public:
    SCHEMA_FIELD(m_bBombTicking, "C_PlantedC4", "m_bBombTicking", bool);
    SCHEMA_FIELD(m_nBombSite, "C_PlantedC4", "m_nBombSite", int32_t);
    SCHEMA_FIELD(m_bCannotBeDefused, "C_PlantedC4", "m_bCannotBeDefused", bool);
    SCHEMA_FIELD(m_bHasExploded, "C_PlantedC4", "m_bHasExploded", bool);
    SCHEMA_FIELD(m_bBeingDefused, "C_PlantedC4", "m_bBeingDefused", bool);
    SCHEMA_FIELD(m_flTimerLength, "C_PlantedC4", "m_flTimerLength", float);
    SCHEMA_FIELD(m_bC4Activated, "C_PlantedC4", "m_bC4Activated", bool);
    SCHEMA_FIELD(m_bTenSecWarning, "C_PlantedC4", "m_bTenSecWarning", bool);
    SCHEMA_FIELD(m_bBombDefused, "C_PlantedC4", "m_bBombDefused", bool);
    SCHEMA_FIELD(m_hBombDefuser, "C_PlantedC4", "m_hBombDefuser", CBaseHandle); // C_CSPlayerPawn
    SCHEMA_FIELD(m_flC4Blow, "C_PlantedC4", "m_flC4Blow", float); // GameTime_t
};

class CPlayer_WeaponServices;
class CPlayer_CameraServices;
class CPlayer_ObserverServices;
class CCSPlayer_ViewModelServices;
class CCSPlayer_ItemServices;
class CCSPlayer_PingServices;
class C_BasePlayerPawn : public C_BaseModelEntity
{
public:
    SCHEMA_FIELD(m_hController, "C_BasePlayerPawn", "m_hController", CBaseHandle);
    SCHEMA_FIELD(v_angle, "C_BasePlayerPawn", "v_angle", Vector_t);
    SCHEMA_FIELD(v_angle_previous, "C_BasePlayerPawn", "v_anglePrevious", Vector_t);
    SCHEMA_FIELD(m_flMouseSensitivity, "C_BasePlayerPawn", "m_flMouseSensitivity", float);
    SCHEMA_FIELD(m_flDetectedByEnemySensorTime, "C_BasePlayerPawn", "m_flDetectedByEnemySensorTime", float);
    SCHEMA_FIELD(m_pWeaponServices, "C_BasePlayerPawn", "m_pWeaponServices", CPlayer_WeaponServices*);
    SCHEMA_FIELD(m_pCameraServices, "C_BasePlayerPawn", "m_pCameraServices", CPlayer_CameraServices*);
    SCHEMA_FIELD(m_pObserverServices, "C_BasePlayerPawn", "m_pObserverServices", CPlayer_ObserverServices*);
    SCHEMA_FIELD(m_pItemServices, "C_BasePlayerPawn", "m_pItemServices", CCSPlayer_ItemServices*);
};

class C_CSPlayerPawnBase : public C_BasePlayerPawn
{
public:
    SCHEMA_FIELD(m_flFlashDuration, "C_CSPlayerPawnBase", "m_flFlashDuration",
        float);
    SCHEMA_FIELD(m_entitySpottedState, "C_CSPlayerPawnBase",
        "m_entitySpottedState", EntitySpottedState_t);
    SCHEMA_FIELD(m_bHasMovedSinceSpawn, "C_CSPlayerPawnBase",
        "m_bHasMovedSinceSpawn", bool);
    SCHEMA_FIELD(m_szLastPlaceName, "C_CSPlayerPawnBase", "m_szLastPlaceName", char[18]);
    SCHEMA_FIELD(m_pViewModelServices, "C_CSPlayerPawnBase", "m_pViewModelServices", CCSPlayer_ViewModelServices*);
    SCHEMA_FIELD(m_ArmorValue, "C_CSPlayerPawnBase", "m_ArmorValue", int32_t);
    SCHEMA_FIELD(m_pPingServices, "C_CSPlayerPawnBase", "m_pPingServices", CCSPlayer_PingServices*);
    SCHEMA_FIELD(m_bIsScoped, "C_CSPlayerPawnBase", "m_bIsScoped", bool);
};

class C_CSPlayerPawn : public C_CSPlayerPawnBase
{
public:
    SCHEMA_FIELD(m_aimPunchCache, "C_CSPlayerPawn", "m_aimPunchCache", CUtlVector<Vector_t>);

    __forceinline bool IsDormant() noexcept {
        CGameSceneNode* game_scene_node = m_pGameSceneNode();

        if (game_scene_node == nullptr) return false;

        return game_scene_node->m_bDormant();
    }

    __forceinline std::uint16_t GetCollisionMask() noexcept {
        if (this && m_pCollision())
            return m_pCollision()->CollisionMask();  // Collision + 0x38

        return 0;
    }

    __forceinline std::uint32_t GetEntityHandle() noexcept {
        typedef std::uint32_t(__fastcall* fn)(void*);
        static auto func = signature::find("client.dll", "48 85 C9 74 ? 48 8B 41 ? 48 85 C0 74 ? 44 8B 40 ? BA ? ? ? ? 8B 48 ? 41 8B C0 83 E1").get<fn>();

        return func(this);
    }

    __forceinline std::uint32_t GetOwnerHandle() noexcept {
        std::uint32_t Result = -1;
        if (this && m_pCollision() && !(m_pCollision()->m_solidFlags() & 4)) {
            Result = this->m_hOwnerEntity().GetEntryIndex();
        }

        return Result;
    }

    __forceinline Vector_t GetEyePosition() noexcept {
        return this->m_pGameSceneNode()->m_vecOrigin() + this->m_vecViewOffset();
    }

    __forceinline Vector_t GetEyeAngles() noexcept {
        Vector_t eye_position;

        CALL_VIRTUAL(std::uint64_t, 160, this, &eye_position);

        return eye_position;
    }
};

class CBasePlayerController : public C_BaseModelEntity
{
public:
    SCHEMA_FIELD(m_steamID, "CBasePlayerController", "m_steamID", uint64_t);
    SCHEMA_FIELD(m_hPawn, "CBasePlayerController", "m_hPawn", CBaseHandle);
    SCHEMA_FIELD(m_bIsLocalPlayerController, "CBasePlayerController",
        "m_bIsLocalPlayerController", bool);
};

class CCSPlayerController : public CBasePlayerController {
public:
    SCHEMA_FIELD(m_sSanitizedPlayerName, "CCSPlayerController",
        "m_sSanitizedPlayerName", const char*);
    SCHEMA_FIELD(m_iPawnHealth, "CCSPlayerController", "m_iPawnHealth", uint32_t);
    SCHEMA_FIELD(m_iPawnArmor, "CCSPlayerController", "m_iPawnArmor", uint32_t);
    SCHEMA_FIELD(m_iDraftIndex, "CCSPlayerController", "m_iDraftIndex", int32_t);
    SCHEMA_FIELD(m_bPawnHasDefuser, "CCSPlayerController", "m_bPawnHasDefuser",
        bool);
    SCHEMA_FIELD(m_bPawnHasHelmet, "CCSPlayerController", "m_bPawnHasHelmet",
        bool);
    SCHEMA_FIELD(m_bPawnIsAlive, "CCSPlayerController", "m_bPawnIsAlive", bool);
    SCHEMA_FIELD(m_bCannotBeKicked, "CCSPlayerController", "m_bCannotBeKicked", bool);
    SCHEMA_FIELD(m_bEverFullyConnected, "CCSPlayerController", "m_bEverFullyConnected", bool);
    SCHEMA_FIELD(m_bControllingBot, "CCSPlayerController", "m_bControllingBot", bool);
    SCHEMA_FIELD(m_iPing, "CCSPlayerController", "m_iPing", uint32_t);

    CCSPlayerController* GetLocalPlayerController() noexcept;
};

class C_BaseViewModel : public C_BaseModelEntity
{
public:
    SCHEMA_FIELD(m_hWeapon, "C_BaseViewModel", "m_hWeapon", CBaseHandle);
};