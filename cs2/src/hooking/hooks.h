#pragma once

namespace hooks
{
	inline DWORD_PTR* pSwapChainVtable = NULL;
	inline HWND window = NULL;
	inline ID3D11Device* p_device = NULL;
	inline ID3D11DeviceContext* p_context = NULL;
	inline ID3D11RenderTargetView* main_render_target_view;

	inline vmt_hook client_hooks;
	inline vmt_hook input_hooks;

	namespace wndproc
	{
		inline WNDPROC original;
		LRESULT __stdcall  wndproc_hook(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	}

	namespace present
	{
		typedef HRESULT(__stdcall* presentfn)(IDXGISwapChain*, UINT, UINT);
		inline presentfn original;

		HRESULT __stdcall present_hook(IDXGISwapChain* swap_chain, UINT sync, UINT flags);
	}

	namespace framestagenotify
	{
		typedef void(__fastcall* framestagefn)(void*, int);
		inline framestagefn original;

		void __fastcall framestage_hk(void* rcx, int stage);
	}

	namespace draw_object
	{
		typedef bool(__fastcall* fn)(void*, void*, material_data_t*, int, void*, void*, void*, void*);
		inline fn original;

		bool __fastcall draw_object(void* animtable_scene_object, void* dx11, material_data_t* data, int data_counter,
			void* scene_view, void* scene_layer, void* unknown_pointer, void* unknown);
	}

	namespace draw_scope
	{
		typedef void(__fastcall* fn)(void*);
		inline fn original;

		void __fastcall draw_scope(void* rcx);
	}

	namespace camera_far
	{
		typedef bool(__fastcall* fn)(void*);
		inline fn original;

		bool __fastcall camera_too_far(void* rcx);
	}

	namespace event_handler
	{
		typedef void(__fastcall* fn)(void*, events_t*);
		inline fn original;

		void __fastcall handle_events_hk(void* rcx, events_t* const event);
	}

	namespace override_view
	{
		typedef void(__fastcall* fn)(void*, CViewSetup*);
		inline fn original;

		void __fastcall override_view(void* rcx, CViewSetup* view_setup);
	}

	namespace get_render_fov
	{
		typedef float(__fastcall* fn)(void*);
		inline fn original;

		float __fastcall get_render_fov_hk(void* rcx);
	}


	namespace get_desired_fov
	{
		typedef float(__fastcall* fn)(void*);
		inline fn original;

		float __fastcall get_desired_fov(void* rcx);
	}

	namespace should_draw_crosshair
	{
		typedef bool(__fastcall* fn)(void*);
		inline fn original;

		bool __fastcall should_draw_crosshair(void* rcx);
	}

	namespace firstperson_legs
	{
		typedef bool(__fastcall* fn)(void*);
		inline fn original;

		bool __fastcall firstperson_legs_hk(void* rcx);
	}

	namespace createmove
	{
		typedef bool(__fastcall* cmovefn)(CCSGOInput*, unsigned int, std::int64_t, std::int8_t);
		inline cmovefn original;

		bool __fastcall createmove_hk(CCSGOInput* input, unsigned int a2, __int64 a3, unsigned __int8 a4);
	}

	namespace level_init
	{
		typedef void* (__fastcall* fn)(void*, const char*);
		inline fn original;

		void* __fastcall level_init_hk(void* rcx, const char* map);
	}

	namespace get_matrices_for_view
	{
		typedef void(__fastcall* fn)(void*, void*, ViewMatrix_t*, ViewMatrix_t*, ViewMatrix_t*, ViewMatrix_t*);
		inline fn original;

		void __fastcall get_matrices_for_view_hk(void* rcx, void* view, ViewMatrix_t* world, ViewMatrix_t* view_proj, ViewMatrix_t* world_proj, ViewMatrix_t* world_pixels);
	}

	namespace mouse_enabled
	{
		typedef bool(__fastcall* mouseenabledfn)(void*);
		inline mouseenabledfn original;

		bool __fastcall mouseinput_hk(void* ptr);
	}

	namespace camera_update
	{
		typedef void(__fastcall* camerafn)(CCSGOInput*, unsigned int);
		inline camerafn original;

		void __fastcall camera_update_hk(CCSGOInput* input, unsigned int a2);
	}

	namespace cvar_value
	{
		typedef void* (__fastcall* cvarfn)(void*, int);
		inline cvarfn original;

		void* __fastcall cvar_value_hk(void* cmd, int default_value);
	}

	namespace get_camera_offsets
	{
		typedef void(__fastcall* fn)(void*, void*, void*, void*);
		inline fn original;

		void __fastcall get_camera_offsets_hk(void* a1, void* a2, void* a3, void* a4);
	}

	void initialize_main();
	void uninitialize();
}