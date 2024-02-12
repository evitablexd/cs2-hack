#pragma once

#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "freetype.lib")

#include <Windows.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <map>
#include <cmath>
#include <array>
#include <algorithm>
#include <stdio.h>
#include <ctime>
#include <unordered_map>
#include <intrin.h>
#include <corecrt_math.h>
#include <set>
#include <d3dx9.h>
#include <optional>
#include <type_traits>
#include <dxgi.h>
#include <d3d11.h>
#include <ostream>
#include <filesystem>
#include <fstream>
#include "sdk/json.h"

/* libs for directx */
#pragma comment(lib, "freetype.lib")

// padding macro, please use, counts pads in class automaticly
#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define PAD( size ) uint8_t MACRO_CONCAT( _pad, __COUNTER__ )[ size ];

#include "sdk/signatures.h"
#include "thirdparty/imgui.h"
#include "thirdparty/imgui_impl_dx11.h"
#include "thirdparty/imgui_impl_win32.h"
#include "thirdparty/imgui_internal.h"
#include "thirdparty/imgui_freetype.h"
#include "thirdparty/minhook/include/MinHook.h"
#include "hooking/vmt.h"

#include "sdk/bitflag.h"
#include "virtual.h"
#include "sdk/math.h"
#include "sdk/crt.h"
#include "fnv.h"
#include "menu/keybinds.h"
#include "sdk/datatypes/color.h"
#include "notify.h"
#include "configs/cfg.h"
#include "tinyformat.h"

#include "sdk/datatypes/Vector.h"
#include "sdk/datatypes/matrix.h"
#include "sdk/datatypes/QAngle.h"
#include "sdk/tier0/memalloc.h"
#include "menu/framework/mui.h"
#include "menu/menu.h"
#include "sdk/common/CStrongHandle.h"
#include "sdk/common/CUtlSymbolLarge.h"
#include "sdk/common/CUtlMemory.h"
#include "sdk/common/CUtlMap.h"
#include "sdk/common/CUtlString.h"
#include "sdk/common/CUtlTSHash.h"
#include "sdk/common/CUtlVector.h"
#include "sdk/datatypes/CUserCmd.h"
#include "sdk/datatypes/CCSGOInput.h"
#include "sdk/datatypes/cviewsetup.h"
#include "sdk/datatypes/Traces.h"
#include "sdk/chandle/chandle.h"
#include "sdk/materialsystem/materialsystem.h"
#include "sdk/datatypes/EventManager.h"
#include "hooking/hooks.h"
#include "sdk/schemasystem/schema.h"
#include "sdk/ccvar/ccvar.h"
#include "sdk/inputsystem/inputsystem.h"
#include "sdk/gameresource/gameresource.h"
#include "sdk/engine/engineclient.h"
#include "sdk/engine/globalvars.h"

#include "sdk/general/collisionproperty.h"
#include "sdk/general/gamescene.h"
#include "sdk/general/entity.h"
#include "sdk/general/player.h"
#include "sdk/general/services.h"
#include "sdk/general/inventorymanager.h"
#include "sdk/general/weapon.h"

#include "sdk/render/render.h"
#include "features/autowall/autowall.h"
#include "features/misc/miscellaneous.h"
#include "features/visuals/esp.h"
#include "features/aimbot/legitbot.h"

namespace sdk
{
    using InstantiateInterfaceFn = void* (*)();

    class InterfaceReg {
    public:
        InstantiateInterfaceFn m_create_fn_;
        const char* m_name_;
        InterfaceReg* m_next_;
    };

    inline const InterfaceReg* GetInterfaces(const char* library) {
        const auto library_handle = GetModuleHandleA(library);

        const auto createinterface_symbol = reinterpret_cast<std::uintptr_t>(GetProcAddress(library_handle, "CreateInterface"));

        if (!createinterface_symbol)
            return nullptr;

        const uintptr_t interface_list = createinterface_symbol + *reinterpret_cast<int32_t*>(createinterface_symbol + 3) + 7;

        return *reinterpret_cast<InterfaceReg**>(interface_list);
    }

    template <typename T = void*>
    T* GetInterface(const char* library, const char* partial_version) {
        for (const InterfaceReg* current = GetInterfaces(library); current; current = current->m_next_) {
            if (std::string_view(current->m_name_).find(partial_version) != std::string_view::npos)
                return reinterpret_cast<T*>(current->m_create_fn_());
        }

        return nullptr;
    }

    inline CCSPlayerController* local_controller = nullptr;
    inline C_BasePlayerPawn* local_pawn = nullptr;
    inline Matrix3x4_t view_matrix;
}

namespace interfaces
{
	inline void* g_client = nullptr;

    inline CCSGOInput* g_input = nullptr;
	inline CSchemaSystem* g_schema = nullptr;
    inline CEngineClient* g_engine = nullptr;
    inline CGameResourceService* g_game_resouce = nullptr;
    inline CCvar* g_cvar = nullptr;
    inline CInputSystem* g_input_system = nullptr;
    inline CMaterialSystem2* g_material_system = nullptr;
    inline CGameTraceManager* g_trace_manager = nullptr;
    inline CLocalize* g_localize = nullptr;
    inline IGlobalVariables* g_global_vars = nullptr;
    inline EventManager* g_event_manager = nullptr;
    inline void* g_pPvsManager = nullptr;
}
