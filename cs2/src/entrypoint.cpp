#include "includes.h"

void thread(void* module)
{
	printf("allocated main thread at %p\n", module);

	// initialize math exports
	math::Setup();

	interfaces::g_client = sdk::GetInterface<void>("client.dll", "Source2Client002");
	printf("Source2Client002 -> %p\n", interfaces::g_client);

	interfaces::g_schema = sdk::GetInterface<CSchemaSystem>("schemasystem.dll", "SchemaSystem_0");
	printf("SchemaSystem_0 -> %p\n", reinterpret_cast<void*>(interfaces::g_schema));

	interfaces::g_engine = sdk::GetInterface<CEngineClient>("engine2.dll", "Source2EngineToClient001");
	printf("Source2EngineToClient001 -> %p\n", reinterpret_cast<void*>(interfaces::g_engine));

	interfaces::g_game_resouce = sdk::GetInterface<CGameResourceService>("engine2.dll", "GameResourceServiceClientV001");
	printf("GameResourceServiceClientV001 -> %p\n", reinterpret_cast<void*>(interfaces::g_game_resouce));

	interfaces::g_cvar = sdk::GetInterface<CCvar>("tier0.dll", "VEngineCvar007");
	printf("VEngineCvar007 -> %p\n", reinterpret_cast<void*>(interfaces::g_cvar));

	interfaces::g_input_system = sdk::GetInterface<CInputSystem>("inputsystem.dll", "InputSystemVersion001");
	printf("InputSystemVersion001 -> %p\n", reinterpret_cast<void*>(interfaces::g_input_system));

	interfaces::g_material_system = sdk::GetInterface<CMaterialSystem2>("materialsystem2.dll", "VMaterialSystem2_001");
	printf("VMaterialSystem2_001 -> %p\n", reinterpret_cast<void*>(interfaces::g_material_system));

	interfaces::g_localize = sdk::GetInterface<CLocalize>("localize.dll", "Localize_001");
	printf("Localize_001 -> %p\n", reinterpret_cast<void*>(interfaces::g_localize));

	interfaces::g_input = signature::find("client.dll", "48 8B 0D ? ? ? ? E8 ? ? ? ? 8B BE ? ? ? ? 44 8B F0 85 FF 78 04 FF C7 EB 03").self_rva(0x3u, 0x7u).deref().get<CCSGOInput*>();
	printf("CCSGOInput -> %p\n", interfaces::g_input);

	interfaces::g_trace_manager = signature::find("client.dll", "4C 8B 3D ? ? ? ? 24 C9 0C 49 66 0F 7F 45 ?").self_rva(0x3u, 0x7u).deref().get<CGameTraceManager*>();
	printf("CGameTraceManager -> %p\n", interfaces::g_trace_manager);
	
	interfaces::g_global_vars = signature::find("client.dll", "48 89 0D ? ? ? ? 48 89 41").self_rva(0x3u, 0x7u).deref().get<IGlobalVariables*>();
	printf("CGlobalVars -> %p\n", interfaces::g_global_vars);

	interfaces::g_event_manager = signature::find("client.dll", "48 89 05 ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 8B E0 00 00 00").self_rva(0x3u, 0x7u).deref().get<EventManager*>();
	printf("EventManager -> %p\n", interfaces::g_event_manager);

	interfaces::g_pPvsManager = signature::find("engine2.dll", "48 8D 0D ?? ?? ?? ?? 33 D2 FF 50").self_rva(0x3u, 0x7u).get<void*>();
	printf("PVSManager -> %p\n", interfaces::g_pPvsManager);


	hooks::initialize_main();

	if (MH_EnableHook(MH_ALL_HOOKS) == MH_STATUS::MH_OK)
		printf("enabled all hooks\n");

}

void unload(void* module)
{
	printf("allocated unload thread at %p\n", module);

	while (!GetAsyncKeyState(VK_END))
		Sleep(100);

	hooks::uninitialize();

	// reset wndproc
	SetWindowLongPtrA(hooks::window, GWLP_WNDPROC, LONG_PTR(hooks::wndproc::original));

	if (MH_Uninitialize() == MH_STATUS::MH_OK)
		printf("uninitialized hooks\n");

	FreeConsole();
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(module), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(void* module, unsigned long reason, void* reserved)
{
	if (reason != DLL_PROCESS_ATTACH)
		return FALSE;

	DisableThreadLibraryCalls(reinterpret_cast<HMODULE>(module));

	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);

	if (MH_Initialize() == MH_STATUS::MH_OK)
		printf("initialized minhook\n");

	void* handle = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread), module, NULL, NULL);
	if (handle) CloseHandle(handle);

	void* unload_handle = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(unload), module, NULL, NULL);
	if (unload_handle) CloseHandle(unload_handle);

	return TRUE;
}

