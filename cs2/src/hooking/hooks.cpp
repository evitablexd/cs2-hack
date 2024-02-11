#include "../includes.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hooks::wndproc::wndproc_hook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CHAR:
	{
		wchar_t wch;
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (char*)&wparam, 1, &wch, 1);
		ImGui::GetIO().AddInputCharacter(wch);

		return ::CallWindowProcA(original, hwnd, msg, wparam, lparam);
	}

	case WM_XBUTTONDOWN:

		switch (GET_KEYSTATE_WPARAM(wparam))
		{
		case MK_XBUTTON1: g_key_binds.m_last_code = VK_XBUTTON1; break;
		case MK_XBUTTON2: g_key_binds.m_last_code = VK_XBUTTON2; break;
		}

		break;

	case WM_MBUTTONDOWN:

		g_key_binds.m_last_code = VK_MBUTTON;
		break;

	case WM_RBUTTONDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:

		g_key_binds.m_last_code = wparam;
		break;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
		g_menu.is_opened() = !g_menu.is_opened();

	if (g_menu.is_opened())
		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

	// call original
	return ::CallWindowProcA(original, hwnd, msg, wparam, lparam);
}

void init_fonts()
{
	auto& io = ImGui::GetIO();

	io.IniFilename = io.LogFilename = nullptr;

	ImFontConfig menu_elements_cfg;
	menu_elements_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
	menu_elements_cfg.OversampleH = menu_elements_cfg.OversampleV = 3;
	menu_elements_cfg.PixelSnapH = false;
	menu_elements_cfg.RasterizerMultiply = 1.3f;

	ImFontConfig menu_tabs_cfg;
	menu_tabs_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
	menu_tabs_cfg.OversampleH = menu_tabs_cfg.OversampleV = 3;
	menu_tabs_cfg.PixelSnapH = false;
	menu_tabs_cfg.RasterizerMultiply = 1.2f;

	ImFontConfig nicknametop_tabs_cfg;
	menu_tabs_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_NoHinting;
	menu_tabs_cfg.OversampleH = menu_tabs_cfg.OversampleV = 3;
	menu_tabs_cfg.PixelSnapH = false;
	menu_tabs_cfg.RasterizerMultiply = 1.3f;

	ImFontConfig for_esp_shit;
	for_esp_shit.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint;
	for_esp_shit.OversampleH = menu_tabs_cfg.OversampleV = 3;
	for_esp_shit.PixelSnapH = false;
	for_esp_shit.RasterizerMultiply = 1.2f;

	ImFontConfig esp_cfg;
	esp_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Monochrome | ImGuiFreeTypeBuilderFlags_MonoHinting;
	esp_cfg.OversampleH = esp_cfg.OversampleV = 5;
	esp_cfg.PixelSnapH = false;
	esp_cfg.RasterizerMultiply = 1.2f;

	ImFontConfig ind_cfg;
	ind_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_Bold;
	ind_cfg.OversampleH = ind_cfg.OversampleV = 5;
	ind_cfg.PixelSnapH = false;
	ind_cfg.RasterizerMultiply = 1.2f;

	render::m_verdana = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 12.f, &esp_cfg, io.Fonts->GetGlyphRangesCyrillic());
	render::m_tahoma14 = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Tahoma.ttf"), 13.f, &menu_elements_cfg, io.Fonts->GetGlyphRangesCyrillic());
	//render::m_acta_symbols = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\acta.ttf"), 26.f, &menu_elements_cfg, io.Fonts->GetGlyphRangesCyrillic());
	render::m_indicator_font = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Verdana.ttf"), 26.f, &ind_cfg, io.Fonts->GetGlyphRangesCyrillic());
}

HRESULT __stdcall hooks::present::present_hook(IDXGISwapChain* swap_chain, UINT sync, UINT flags)
{
	static bool initialized{ false };
	if (!initialized)
	{
		if (SUCCEEDED(swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&p_device)))) {
			p_device->GetImmediateContext(&p_context);

			auto swapchain_desc = DXGI_SWAP_CHAIN_DESC{};
			swap_chain->GetDesc(&swapchain_desc);
			window = swapchain_desc.OutputWindow;

			ID3D11Texture2D* back_buffer{};
			swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
			p_device->CreateRenderTargetView(back_buffer, nullptr, &main_render_target_view);
			back_buffer->Release();

			wndproc::original = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)wndproc::wndproc_hook);

			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(p_device, p_context);
			ImGui::GetStyle().WindowMinSize = ImVec2(500, 350);

			init_fonts();

			initialized = true;
		}
		else {
			return original(swap_chain, sync, flags);
		}
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (g_menu.is_opened()) {
		ImGui::Begin("cheat", 0, ImGuiWindowFlags_NoCollapse);

		g_menu.render();

		ImGui::End();
	}

	render::drawList = ImGui::GetBackgroundDrawList();

	esp::render();

	ImGui::EndFrame();
	ImGui::Render();

	p_context->OMSetRenderTargets(1, &main_render_target_view, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return original(swap_chain, sync, flags);
}

void __fastcall hooks::framestagenotify::framestage_hk(void* rcx, int stage)
{
	sdk::local_controller = interfaces::g_game_resouce->entity_system->GetLocalPlayerController();
	sdk::local_pawn = sdk::local_controller != nullptr ? interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(sdk::local_controller->m_hPawn()) : nullptr;

	esp::render_fsn(stage);

	if (stage == FRAME_RENDER_START)
	{
		if (interfaces::g_input) {
			interfaces::g_input->bInThirdPerson = g_key_binds.get_keybind_state(&g_variables.thirdperson_key);

			ConVar* dist = interfaces::g_cvar->FindVarByName("cam_idealdist");
			if (dist)
				dist->GetValue<float>() = g_variables.distance;
		}
	}

	if (stage == FRAME_RENDER_END)
		miscellaneous::skin_changer();

	// call original
	return original(rcx, stage);
}

void hooks::initialize_main()
{
	static auto present_overlay = signature::find("GameOverlayRenderer64.dll", "48 89 6C 24 ? 48 89 74 24 ? 41 56 48 83 EC 20 41 8B E8").get<void*>();
	MH_CreateHook(present_overlay, present::present_hook, reinterpret_cast<void**>(&present::original));

	if (client_hooks.initialize(interfaces::g_client)) {
		client_hooks.hook(reinterpret_cast<void*>(hooks::framestagenotify::framestage_hk), 33, &hooks::framestagenotify::original);

		client_hooks.swap_context();
	}

	if (input_hooks.initialize(interfaces::g_input)) {
		input_hooks.hook(reinterpret_cast<void*>(hooks::createmove::createmove_hk), 5, &hooks::createmove::original);

		input_hooks.swap_context();
	}

	MH_CreateHook(signature::find("client.dll", "40 55 53 56 57 41 55 48 8D AC 24 ? ? ? ? 48 81 EC").get<void*>(),
		reinterpret_cast<void*>(hooks::camera_update::camera_update_hk),
		reinterpret_cast<LPVOID*>(&hooks::camera_update::original));

	MH_CreateHook(signature::find("client.dll", "40 53 48 81 EC 80 00 00 00 48 8B D9 E8 ?? ?? ?? ?? 48 85").get<void*>(),
		reinterpret_cast<void*>(hooks::get_render_fov::get_render_fov_hk),
		reinterpret_cast<LPVOID*>(&hooks::get_render_fov::original));

	//MH_CreateHook(signature::find("client.dll", "48 89 5C 24 ? 57 48 83 EC 20 48 8B 41 30 48 8B F9").get<void*>(),
	//	reinterpret_cast<void*>(hooks::get_desired_fov::get_desired_fov),
	//	reinterpret_cast<LPVOID*>(&hooks::get_desired_fov::original));

	MH_CreateHook(signature::find("client.dll", "40 53 48 83 EC 70 48 8B 01").get<void*>(),
		reinterpret_cast<void*>(hooks::camera_far::camera_too_far),
		reinterpret_cast<LPVOID*>(&hooks::camera_far::original));

	//MH_CreateHook(signature::find("client.dll", "40 53 57 41 56 48 83 EC 20 4C 8B F1").get<void*>(),
	//	reinterpret_cast<void*>(hooks::draw_scope::draw_scope),
	//	reinterpret_cast<LPVOID*>(&hooks::draw_scope::original));

	MH_CreateHook(signature::find("client.dll", "48 89 5C 24 ? 56 48 83 EC ? 48 8B 0D ? ? ? ? 48 8B F2").get<void*>(),
		reinterpret_cast<void*>(hooks::level_init::level_init_hk),
		reinterpret_cast<LPVOID*>(&hooks::level_init::original));

	MH_CreateHook(signature::find("client.dll", "E8 ? ? ? ? 84 C0 74 59 49 8B CF").add(0x1).rel32().get<void*>(),
		reinterpret_cast<void*>(hooks::should_draw_crosshair::should_draw_crosshair),
		reinterpret_cast<LPVOID*>(&hooks::should_draw_crosshair::original));

	MH_CreateHook(signature::find("scenesystem.dll", "48 8B C4 48 89 50 10 55 41 56").get<void*>(),
		reinterpret_cast<void*>(hooks::draw_object::draw_object),
		reinterpret_cast<LPVOID*>(&hooks::draw_object::original));

	MH_CreateHook(signature::find("client.dll", "40 55 53 41 55 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 02").get<void*>(),
		reinterpret_cast<void*>(hooks::event_handler::handle_events_hk),
		reinterpret_cast<LPVOID*>(&hooks::event_handler::original));

	MH_CreateHook(signature::find("client.dll", "40 53 48 83 EC 20 48 8B D9 E8 ? ? ? ? 48 85 C0 0F 85 ? ? ? ?").get<void*>(),
		reinterpret_cast<void*>(hooks::firstperson_legs::firstperson_legs_hk),
		reinterpret_cast<LPVOID*>(&hooks::firstperson_legs::original));

	//MH_CreateHook(signature::find("client.dll", "48 83 EC 28 BA ? ? ? ? 48 8D 0D ? ? ? ? E8 ? ? ? ? 48 85 C0 74 09 F3 0F 10 00 48 83 C4 28 C3 48 8B 05 ? ? ? ? 48 8B 48 08 F3 0F 10 01 48 83 C4 28 C3 CC CC CC CC CC CC CC CC CC 85 D2").get<void*>(),
	//	reinterpret_cast<void*>(hooks::get_viewmodel_fov::get_viewmodel_fov_hk),
	//	reinterpret_cast<LPVOID*>(&hooks::get_viewmodel_fov::original));

	MH_CreateHook(signature::find("client.dll", "E8 ? ? ? ? 48 85 C0 75 0B 48 8B 05 ? ? ? ? 48 8B 40 08 44 38 38 75 3B").add(0x1).rel32().get<void*>(),
		reinterpret_cast<void*>(hooks::cvar_value::cvar_value_hk),
		reinterpret_cast<LPVOID*>(&hooks::cvar_value::original));

	// note: apparently changing fov in there doesn't do anything ?

	MH_CreateHook(signature::find("client.dll", "48 89 5C 24 ? 55 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 01").get<void*>(),
		reinterpret_cast<void*>(hooks::override_view::override_view),
		reinterpret_cast<LPVOID*>(&hooks::override_view::original));

	MH_CreateHook(signature::find("client.dll", "40 53 48 81 EC ? ? ? ? 49 8B C1").get<void*>(),
		reinterpret_cast<void*>(hooks::get_matrices_for_view::get_matrices_for_view_hk),
		reinterpret_cast<LPVOID*>(&hooks::get_matrices_for_view::original));
}

void hooks::uninitialize()
{
	client_hooks.unhook();
	input_hooks.unhook();
}

bool __fastcall hooks::createmove::createmove_hk(CCSGOInput* input, unsigned int a2, __int64 a3, unsigned __int8 a4)
{
	const bool result = original(input, a2, a3, a4);

	if (!interfaces::g_engine->IsConnected() || !interfaces::g_engine->IsInGame())
		return result;

	CUserCmd* cmd = input->GetUserCmd();
	if (!cmd)
		return result;

	if (sdk::local_controller && sdk::local_pawn)
	{
		miscellaneous::run(cmd);
	}

	// call original
	return result;
}

bool __fastcall hooks::mouse_enabled::mouseinput_hk(void* ptr)
{
	return g_menu.is_opened() ? false : original(ptr);
}

void __fastcall hooks::camera_update::camera_update_hk(CCSGOInput* input, unsigned int a2)
{
	return original(input, a2);
}

void* __fastcall hooks::cvar_value::cvar_value_hk(void* cmd, int default_value)
{
	// call cvar_value
	// test rax, rax
	static auto camera_think = signature::find("client.dll", "48 85 C0 75 0B 48 8B 05 ? ? ? ? 48 8B 40 08 44 38 38 75 3B").get<void*>();

	static bool fake_cvar = true;
	if (_ReturnAddress() == camera_think)
		return &fake_cvar;

	return original(cmd, default_value);
}

void __fastcall hooks::override_view::override_view(void* rcx, CViewSetup* view_setup)
{
	return original(rcx, view_setup);
}

void __fastcall hooks::get_matrices_for_view::get_matrices_for_view_hk(void* rcx, void* view, ViewMatrix_t* world, ViewMatrix_t* view_proj, ViewMatrix_t* world_proj, ViewMatrix_t* world_pixels)
{
	original(rcx, view, world, view_proj, world_proj, world_pixels);

	// update view matrix
	sdk::view_matrix = world_proj->As3x4();
}

void __fastcall hooks::get_camera_offsets::get_camera_offsets_hk(void* a1, void* a2, void* a3, void* a4)
{
	return original(a1, a2, a3, a4);
}

float __fastcall hooks::get_render_fov::get_render_fov_hk(void* rcx)
{
	if (!sdk::local_pawn || !sdk::local_controller || !g_variables.custom_fov)
		return original(rcx);

	CPlayer_WeaponServices* weapon = sdk::local_pawn->m_pWeaponServices();
	if (!weapon)
		return original(rcx);

	C_CSWeaponBaseGun* weapon_base = weapon->m_hActiveWeapon().Get<C_CSWeaponBaseGun>();
	if (!weapon_base)
		return original(rcx);

	if (g_variables.custom_fov_scoped)
	{
		if (weapon_base->m_zoomLevel() > 0)
			return g_variables.fov;
	}
	else
	{
		if (weapon_base->m_zoomLevel() > 0)
			return original(rcx);
	}

	return g_variables.fov;
}

bool __fastcall hooks::firstperson_legs::firstperson_legs_hk(void* rcx)
{
	// dont do this for other players
	if (rcx != reinterpret_cast<void*>(sdk::local_pawn) || !interfaces::g_input)
		return original(rcx);

	/*float v8;
	typedef bool(__fastcall* fn)(void*);
	static auto is_camera_too_close = signature::find("client.dll", "E8 ? ? ? ? BA ? ? ? ? 84 C0 74 25").add(0x1).rel32().get<fn>();

	if (!is_camera_too_close(sdk::local_pawn))
	{
		typedef void* (__fastcall* fn2)(void*, int);
		static auto render_fn = signature::find("client.dll", "E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 5C 83 ? ? ? ? 0F 2F 05 ? ? ? ? 72 08").add(0x1).rel32().get<fn2>();

		render_fn(&v8, 0);
		if ((float)(v8 - *((float*)rcx + 0x517)) >= 0.2)
			return true;
	}*/

	// ghetto fix
	if (interfaces::g_input->bInThirdPerson)
		return original(rcx);

	return g_variables.firstperson_leg ? false : true;
}

void* __fastcall hooks::level_init::level_init_hk(void* rcx, const char* map)
{
	interfaces::g_global_vars = signature::find("client.dll", "48 89 0D ? ? ? ? 48 89 41").self_rva(0x3u, 0x7u).deref().get<IGlobalVariables*>();
	printf("CGlobalVars -> %p\n", interfaces::g_global_vars);

	return original(rcx, map);
}

bool __fastcall hooks::draw_object::draw_object(void* animtable_scene_object, void* dx11, material_data_t* data, int data_counter, void* scene_view, void* scene_layer, void* unknown_pointer, void* unknown)
{
	static material_t* vis_material = esp::create_material("visible", false, true, true);
	static material_t* invis_material = esp::create_material("invisible", true, true, true);

	col_t vis_color = col_t(g_variables.chams_color[0] * 255.f, g_variables.chams_color[1] * 255.f,
		g_variables.chams_color[2] * 255.f, g_variables.chams_color[3] * 255.f);

	col_t invis_color = col_t(g_variables.invis_chams_color[0] * 255.f, g_variables.invis_chams_color[1] * 255.f,
		g_variables.invis_chams_color[2] * 255.f, g_variables.invis_chams_color[3] * 255.f);

	if(!g_variables.chams)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	if(!data || !data->pscene_animatable_object || !sdk::local_controller || !sdk::local_pawn)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);
	
	CBaseHandle owner = data->pscene_animatable_object->owner_handle;

	auto pEntity = interfaces::g_game_resouce->entity_system->Get<C_BaseEntity>(owner);
	if (pEntity == nullptr)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	SchemaClassInfoData_t* pClassInfo = pEntity->Schema_DynamicBinding();
	if (pClassInfo == nullptr)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	if (strstr(pClassInfo->m_name, "C_CSPlayerPawn") == 0) {

		/* todo: weapon/other chams */

		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);
	}

	auto pPawn = interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(owner);
	if (pPawn == nullptr)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	if (pPawn->m_iTeamNum() == sdk::local_pawn->m_iTeamNum() && pPawn != sdk::local_pawn)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	if(pPawn == sdk::local_pawn && !g_variables.local_chams)
		return original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	data->m_material = invis_material;
	data->color = invis_color;

	original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	data->m_material = vis_material;
	data->color = vis_color;

	original(animtable_scene_object, dx11, data, data_counter, scene_view, scene_layer, unknown_pointer, unknown);

	return true;
}

void __fastcall hooks::event_handler::handle_events_hk(void* rcx, events_t* const event)
{
	switch (const char* event_name{ event->get_name() }; FNV1A::hash_32_fnv1a_const(event_name)) {
	case FNV1A::hash_32_fnv1a_const("player_hurt"): {
		if (CCSPlayerController * local_controller{ sdk::local_controller }) {
			if (CCSPlayerController* controller{ event->get_player_controller("attacker") }; controller == local_controller) {

				// to stuff here.
			}
		}
	} break;
	default:
		break;
	}

	original(rcx, event);
}

bool __fastcall hooks::camera_far::camera_too_far(void* rcx)
{
	if (!sdk::local_controller || !sdk::local_pawn || !g_variables.remove_scope)
		return original(rcx);

	static auto addr = signature::find("client.dll", "84 C0 0F 84 ? ? ? ? 48 8B 0D ? ? ? ? 48 8D 95 ? ? ? ? C6 85 ? ? ? ? ? 48 8B 01").get<void*>();

	// don't return false if in this function otherwise your arm will show
	if (_ReturnAddress() == addr)
		return original(rcx);

	if (reinterpret_cast<C_CSPlayerPawn*>(sdk::local_pawn)->m_bIsScoped())
		return false;

	return original(rcx);
}

/*int __fastcall hooks::get_weapon_type::get_weapon_type(void* rcx)
{
	if (!sdk::local_controller || !sdk::local_pawn || !g_variables.force_crosshair)
		return original(rcx);

	if (!sdk::local_controller->m_bPawnIsAlive())
		return original(rcx);

	static auto addr = signature::find("client.dll", "83 F8 05 74 7C").get<void*>();

	// fake xhair
	if (_ReturnAddress() == addr && !reinterpret_cast<C_CSPlayerPawn*>(sdk::local_pawn)->m_bIsScoped())
		return 1;

	return original(rcx);
}*/

float __fastcall hooks::get_desired_fov::get_desired_fov(void* rcx)
{
	return 150.f;
}

void __fastcall hooks::draw_scope::draw_scope(void* rcx)
{
	if (!g_variables.remove_scope || !sdk::local_controller || !sdk::local_pawn)
		return original(rcx);

	// dont draw scope
	if (reinterpret_cast<C_CSPlayerPawn*>(sdk::local_pawn)->m_bIsScoped())
		return;

	return original(rcx);
}

bool __fastcall hooks::should_draw_crosshair::should_draw_crosshair(void* rcx)
{
	if (!g_variables.force_crosshair || !sdk::local_controller || !sdk::local_pawn)
		return original(rcx);

	if (!reinterpret_cast<C_CSPlayerPawn*>(sdk::local_pawn)->m_bIsScoped())
		return true;

	return original(rcx);
}
