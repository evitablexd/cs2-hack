#include "../../includes.h"

RECT esp::get_bbox(C_BasePlayerPawn* ent, bool is_valid)
{
	float x{ }, y{ }, w{ }, h{ };

	Vector_t pos = ent->GetInterpolatedOrigin();

	CCollisionProperty* pCollision = ent->m_pCollision();
	if (!pCollision) {
		return RECT{ };
	}

	Vector_t top = pos + Vector_t(0, 0, pCollision->m_vecMaxs().z);

	Vector_t pos_screen{ }, top_screen{ };

	if (!render::world_to_screen(pos, pos_screen) ||
		!render::world_to_screen(top, top_screen)) {
		is_valid = false;
	}

	x = int(top_screen.x - ((pos_screen.y - top_screen.y) / 2) / 2);
	y = int(top_screen.y);

	w = int(((pos_screen.y - top_screen.y)) / 2);
	h = int((pos_screen.y - top_screen.y));

	const bool out_of_fov = pos_screen.x + w + 20 < 0 || pos_screen.x - w - 20 > 1920 || pos_screen.y + 20 < 0 || pos_screen.y - h - 20 > 1080;

	is_valid = !out_of_fov;
	return RECT{ long(x), long(y), long(x + w), long(y + h) };
}

void esp::render()
{
	if (!render::drawList)
		return;

	col_t name_color = col_t(g_variables.name_color[0] * 255.f, g_variables.name_color[1] * 255.f,
		g_variables.name_color[2] * 255.f, (g_variables.name_color[3] * 255.f));

	col_t box_color = col_t(g_variables.box_color[0] * 255.f, g_variables.box_color[1] * 255.f,
		g_variables.box_color[2] * 255.f, (g_variables.box_color[3] * 255.f));

	if (!interfaces::g_engine->IsInGame()) return;
	if (!sdk::local_controller || !sdk::local_pawn) return;

	// draw scope lines
	if (g_variables.remove_scope && reinterpret_cast<C_CSPlayerPawn*>(sdk::local_pawn)->m_bIsScoped())
	{
		render::drawList->AddLine(ImVec2(0, ImGui::GetIO().DisplaySize.y / 2), ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y / 2), ImColor(0, 0, 0, 255));
		render::drawList->AddLine(ImVec2(ImGui::GetIO().DisplaySize.x / 2, 0), ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y), ImColor(0, 0, 0, 255));
	}

	static float hp_array[64]{ };
	static float ammo_array[64]{ 0.f };

	for (int i = 1; i <= interfaces::g_game_resouce->entity_system->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = interfaces::g_game_resouce->entity_system->Get(i);
		if (!entity || !entity->IsBasePlayerController() || entity->IsBasePlayerWeapon()) continue;

		CCSPlayerController* controller = interfaces::g_game_resouce->entity_system->Get<CCSPlayerController>(entity->GetRefEHandle());
		if (!controller) { continue; }

		if (controller == sdk::local_controller || controller->m_iTeamNum() == sdk::local_controller->m_iTeamNum()) continue;

		C_CSPlayerPawn* pawn = interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(controller->m_hPawn());
		if (!pawn || pawn == sdk::local_pawn) { continue; }

		if (!controller->m_bPawnIsAlive()) {
			hp_array[controller->GetRefEHandle().GetEntryIndex()] = 0.f;
			ammo_array[controller->GetRefEHandle().GetEntryIndex()] = 0.f;
			continue;
		}

		bool valid_bbox{ true };

		RECT rect = get_bbox(pawn, valid_bbox);

		if (!valid_bbox)
			continue;

		if (g_variables.box_esp)
		{
			render::rect(Vector2D_t(rect.left + 1, rect.top + 1), Vector2D_t(rect.right - 1, rect.bottom - 1), col_t(0, 0, 0, 255));
			render::rect(Vector2D_t(rect.left - 1, rect.top - 1), Vector2D_t(rect.right + 1, rect.bottom + 1), col_t(0, 0, 0, 255));
			render::rect(Vector2D_t(rect.left, rect.top), Vector2D_t(rect.right, rect.bottom), box_color);
		}

		if (g_variables.name_esp) {

			std::string name{ controller->m_sSanitizedPlayerName() };

			if (name.length() > 36)
			{
				name.erase(36, name.length() - 36);
				name.append(("..."));
			}

			auto width = abs(rect.right - rect.left);
			auto size = render::m_verdana->CalcTextSizeA(12.f, FLT_MAX, NULL, name.c_str());

			render::text(name, Vector2D_t(rect.left + width * 0.5f, rect.top - size.y - 2), name_color, render::m_verdana, false, true, false, false, true);
		}

		if (!g_variables.healthbar_esp)
			hp_array[controller->GetRefEHandle().GetEntryIndex()] = 0.f;

		if (g_variables.healthbar_esp)
		{
			int Index = controller->GetRefEHandle().GetEntryIndex();

			float box_height = static_cast<float> (rect.bottom - rect.top);

			float health_multiplier = 12.f / 360.f;
			health_multiplier *= std::ceil(pawn->m_iHealth() / 10.f) - 1;

			col_t color = col_t::from_hsb(health_multiplier, 1, 1).alpha(255);

			if (hp_array[Index] > pawn->m_iHealth()) {
				hp_array[Index] = std::lerp(hp_array[Index], pawn->m_iHealth(), interfaces::g_global_vars->m_flFrameTime * 16.f);
			}
			else {
				hp_array[Index] = pawn->m_iHealth();
			}

			float colored_bar_height = ((box_height * std::fmin(hp_array[Index], 100.f)) / 100.0f);
			float colored_max_bar_height = ((box_height * 100.0f) / 100.0f);

			render::rect_filled(Vector2D_t(rect.left - 6.0f, rect.top - 1), Vector2D_t(rect.left - 2.0f, rect.top + colored_max_bar_height + 1), col_t(0.0f, 0.0f, 0.0f, 120.f));
			render::rect_filled(Vector2D_t(rect.left - 5.0f, rect.top + (colored_max_bar_height - colored_bar_height)), Vector2D_t(rect.left - 3.0f, rect.top + colored_max_bar_height), color);

			if (pawn->m_iHealth() <= 92
				|| pawn->m_iHealth() > 100)
			{
				render::text(std::to_string(pawn->m_iHealth()), Vector2D_t(rect.left - 5.f, (rect.top + (colored_max_bar_height - colored_bar_height) - 8)),
					col_t(255, 255, 255, 255), render::m_verdana, false, true, false, false, true);
			}
		}

		if (g_variables.weapon_text_esp) {
			CPlayer_WeaponServices* pWeaponServices = pawn->m_pWeaponServices();
			if (pWeaponServices) {
				C_CSWeaponBase* pActiveWeapon = pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBase>();
				if (pActiveWeapon) {

					C_AttributeContainer pAttributeContainer = pActiveWeapon->m_AttributeManager();
					C_EconItemView pItemView = pAttributeContainer.m_Item();

					CCStrike15ItemDefinition* pItemStaticData = pItemView.GetStaticData();
					if (!pItemStaticData) continue;

					const char* szWeaponName = interfaces::g_localize->FindSafe(pItemStaticData->m_szItemLocalizedName);
					if (!szWeaponName || strlen(szWeaponName) < 1) continue;

					render::text(szWeaponName, Vector2D_t(rect.left + (abs(rect.right - rect.left) * 0.5f), rect.bottom), col_t(255, 255, 255, 255),
						render::m_verdana, false, true, false, false, true);
				}
			}
		}

		if (!g_variables.ammobar_esp)
			ammo_array[controller->GetRefEHandle().GetEntryIndex()] = 0.f;

		if (g_variables.ammobar_esp)
		{
			int plr_idx = controller->GetRefEHandle().GetEntryIndex();

			if (ammo_array[plr_idx] < 1.f) {
				ammo_array[plr_idx] = std::lerp(ammo_array[plr_idx], 1.f, interfaces::g_global_vars->m_flFrameTime * 10.f);
			}
			else {
				ammo_array[plr_idx] = std::lerp(ammo_array[plr_idx], 1.f, interfaces::g_global_vars->m_flFrameTime * 16.f); // make sure x2
			}

			ammo_array[plr_idx] = std::clamp(ammo_array[plr_idx], 0.f, 1.f);

			CPlayer_WeaponServices* pWeaponServices = pawn->m_pWeaponServices();
			if (!pWeaponServices) continue;

			C_CSWeaponBaseGun* pActiveWeapon = pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBaseGun>();
			if (!pActiveWeapon) continue;

			float box_width = std::abs(rect.right - rect.left);
			float current_box_width = (box_width * pActiveWeapon->m_iClip1() / pActiveWeapon->GetWeaponVData()->m_iMaxClip1());

			float size = (current_box_width * ammo_array[plr_idx]);

			// background
			render::rect_filled(Vector2D_t(rect.right + 1, rect.bottom + 2), Vector2D_t(rect.left - 1, rect.bottom + 6), col_t(0.f, 0.f, 0.f, 120.f));

			// actual bar
			render::rect_filled(Vector2D_t(rect.left, rect.bottom + 3), Vector2D_t(rect.left + size, rect.bottom + 5), col_t(95, 174, 227, 255));

			// less than 90% ammo
			if (pActiveWeapon->m_iClip1() < (pActiveWeapon->GetWeaponVData()->m_iMaxClip1() * 0.9))
				render::text(std::to_string(pActiveWeapon->m_iClip1()), Vector2D_t(rect.left + size, rect.bottom - 2),
					col_t(255, 255, 255, 255), render::m_verdana, false, false, false, false, true);
		}

		if (g_variables.flags_esp)
		{
			std::vector < flags_data_t > flags_data{ };
			int idx = controller->GetRefEHandle().GetEntryIndex();

			CCSPlayer_ItemServices* item_serv = pawn->m_pItemServices();
			if (item_serv && pawn->m_ArmorValue() > 0)
			{
				std::string text;
				col_t  clr;

				if (item_serv->m_bHasHelmet()) {
					text = "h";
					clr = col_t(240, 240, 240);
				}
				else {
					text = "k";
					clr = col_t(240, 240, 240);
				}

				flags_data.push_back({ text, clr });
			}

			if (pawn->m_bIsScoped())
				flags_data.push_back({ "zoom", col_t(0, 175, 255, 255) });

			flags_data.push_back({ std::to_string(controller->m_iPing()) + "ms", controller->m_iPing() < 70 ? col_t(255, 255, 255) :
				controller->m_iPing() > 250 ? col_t(217, 39, 39) : col_t(255, 145, 0) });

			if (pawn->m_flFlashDuration() > 0.f)
				flags_data.push_back({ "flashed", col_t(52, 79, 235, 255) });

			// iterate flags.
			for (int i{ }; i < flags_data.size(); ++i) {
				// get flag job ( pair ).
				const auto& f = flags_data[i];

				int offset = i * 11;

				// draw flag.
				col_t clr = col_t(f.m_clr.r(), f.m_clr.g(), f.m_clr.b(), f.m_clr.a());

				render::text(f.m_name, Vector2D_t(rect.right + 5, rect.top + offset - 1), clr, render::m_verdana, false, false, false, false, true);
			}
		}

		if (g_variables.skeleton_esp) {
			CGameSceneNode* game_scene_node = pawn->m_pGameSceneNode();
			if (!game_scene_node) continue;

			CSkeletonInstance* skeleton_instance = game_scene_node->GetSkeletonInstance();
			if (!skeleton_instance) continue;

			CModelState model_state = skeleton_instance->m_modelState();

			const CStrongHandle<CModel> model = model_state.modelHandle;
			if (!model.is_valid()) continue;

			for (std::int32_t i = 0; i < model->BoneCount; ++i)
			{
				const BitFlag bone_flag = model->GetBoneFlags(i);

				// Check if the current bone has the hitbox flag set.
				if (!bone_flag.has_flag(static_cast<std::uint32_t>(FLAG_HITBOX))) continue;

				const std::int32_t bone_parent_index = model->GetBoneParent(i);

				// Skip bones with no parent.
				if (bone_parent_index == -1) continue;

				Vector_t bone_screen_position;
				if (!render::world_to_screen(model_state.bones[i].position, bone_screen_position))
					continue;

				Vector_t bone_screen_parent_position;
				if (!render::world_to_screen(model_state.bones[bone_parent_index].position, bone_screen_parent_position))
					continue;

				col_t clr = col_t(g_variables.skeleton_color[0] * 255.f, g_variables.skeleton_color[1] * 255.f,
					g_variables.skeleton_color[2] * 255.f, g_variables.skeleton_color[3] * 255.f);

				render::line(Vector2D_t(bone_screen_position.x, bone_screen_position.y),
					Vector2D_t(bone_screen_parent_position.x, bone_screen_parent_position.y), clr);
			}
		}
	}

	for (int i = 64; i <= interfaces::g_game_resouce->entity_system->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = interfaces::g_game_resouce->entity_system->Get(i);
		if (!entity)
			continue;

		SchemaClassInfoData_t* scheme = entity->Schema_DynamicBinding();
		if (!scheme)
			continue;

		switch (FNV1A::hash_32_fnv1a_const(scheme->m_name))
		{
		case FNV1A::hash_32_fnv1a_const("C_SmokeGrenadeProjectile"):
		{
			C_SmokeGrenadeProjectile* smoke = reinterpret_cast<C_SmokeGrenadeProjectile*>(entity);

			if (g_variables.custom_smoke)
				smoke->m_vSmokeColor() = Vector_t(g_variables.smoke_color[0] * 255.f, g_variables.smoke_color[1] * 255.f, g_variables.smoke_color[2] * 255.f);

			if (g_variables.smoke_rendering)
				smoke->m_nSmokeEffectTickBegin() = -1;

			Vector_t smoke_position = smoke->GetOrigin();
			Vector_t smoke_screen;
			if (!render::world_to_screen(smoke_position, smoke_screen)) continue;

			render::text("Smoke", Vector2D_t(smoke_screen.x, smoke_screen.y), col_t(255, 255, 255, 255), render::m_verdana, true, true, false, false, true);
			break;
		}
		case FNV1A::hash_32_fnv1a_const("C_HEGrenadeProjectile"):
		{
			C_BaseGrenade* grenade = reinterpret_cast<C_BaseGrenade*>(entity);

			Vector_t position = grenade->GetOrigin();
			Vector_t screen;
			if (!render::world_to_screen(position, screen)) continue;

			render::text("HE Grenade", Vector2D_t(screen.x, screen.y), col_t(255, 255, 255, 255), render::m_verdana, true, true, false, false, true);

			break;
		}
		case FNV1A::hash_32_fnv1a_const("C_FlashbangProjectile"):
		{
			C_BaseGrenade* grenade = reinterpret_cast<C_BaseGrenade*>(entity);

			Vector_t position = grenade->GetOrigin();
			Vector_t screen;
			if (!render::world_to_screen(position, screen)) continue;

			render::text("Flashbang", Vector2D_t(screen.x, screen.y), col_t(255, 255, 255, 255), render::m_verdana, true, true, false, false, true);

			break;
		}
		case FNV1A::hash_32_fnv1a_const("C_MolotovProjectile"):
		{
			C_BaseGrenade* grenade = reinterpret_cast<C_BaseGrenade*>(entity);

			Vector_t position = grenade->GetOrigin();
			Vector_t screen;
			if (!render::world_to_screen(position, screen)) continue;

			render::text("Molotov", Vector2D_t(screen.x, screen.y), col_t(255, 255, 255, 255), render::m_verdana, true, true, false, false, true);

			break;
		}
		case FNV1A::hash_32_fnv1a_const("C_PlantedC4"):
		{
			C_PlantedC4* c4 = reinterpret_cast<C_PlantedC4*>(entity);

			int offset = 0;
			Vector_t pos = c4->GetInterpolatedOrigin();
			Vector_t screen;

			// glow
			c4->GetGlowProperty().m_bGlowing() = g_variables.bomb_glow && !c4->m_bHasExploded() && !c4->m_bBombDefused();
			c4->GetGlowProperty().m_iGlowType() = 3;
			c4->GetGlowProperty().m_glowColorOverride() = col_t(255, 0, 0, 230);

			// if not on screen, too far, or already exploded
			if (!render::world_to_screen(pos, screen) || (c4->GetInterpolatedOrigin() - sdk::local_pawn->GetInterpolatedOrigin()).Length() > 2000.f
				|| c4->m_bHasExploded()
				|| c4->m_bBombDefused()
				|| !g_variables.bomb_text)
				continue;

			C_CSPlayerPawn* defuser_pawn = interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(c4->m_hBombDefuser());

			auto dist_world = (c4->GetInterpolatedOrigin() - sdk::local_pawn->GetInterpolatedOrigin()).Length();
			auto alpha = std::clamp((750.f - (dist_world - 250.f)) / 750.f, 0.f, 1.f);

			// draw main text
			render::text("BOMB", Vector2D_t(screen.x, screen.y), c4->m_bBeingDefused() ? col_t(255, 0, 0, g_variables.bomb_text_fade ? 255 * alpha : 255) :
				col_t(150, 200, 60, g_variables.bomb_text_fade ? 255 * alpha : 255), render::m_verdana, false, true, true, false, true);

			// draw timer
			//render::text(std::to_string(std::abs(timer -= interfaces::g_global_vars->flCurtime)).c_str(), Vector2D_t(screen.x, screen.y + 10 + offset), col_t(255, 0, 0, 255 * alpha), render::m_verdana, false, true, true, false, true);
		}
		break;
		}
	}
}

void esp::render_fsn(int stage)
{
	if (!sdk::local_controller || !sdk::local_pawn)
		return;

	if (stage != FRAME_RENDER_START)
		return;

	col_t glow_color = col_t(g_variables.glow_color[0] * 255.f, g_variables.glow_color[1] * 255.f,
		g_variables.glow_color[2] * 255.f, (g_variables.glow_color[3] * 255.f));

	for (int i = 1; i <= interfaces::g_game_resouce->entity_system->GetHighestEntityIndex(); i++)
	{
		C_BaseEntity* entity = interfaces::g_game_resouce->entity_system->Get(i);
		if (!entity || !entity->IsBasePlayerController() || entity->IsBasePlayerWeapon()) continue;

		CCSPlayerController* controller = interfaces::g_game_resouce->entity_system->Get<CCSPlayerController>(entity->GetRefEHandle());
		if (!controller) { continue; }

		if (controller == sdk::local_controller || controller->m_iTeamNum() == sdk::local_controller->m_iTeamNum()
			|| !controller->m_bPawnIsAlive()) continue;

		C_CSPlayerPawn* pawn = interfaces::g_game_resouce->entity_system->Get<C_CSPlayerPawn>(controller->m_hPawn());
		if (!pawn || pawn == sdk::local_pawn) { continue; }

		pawn->GetGlowProperty().m_bGlowing() = g_variables.glow;
		pawn->GetGlowProperty().m_glowColorOverride() = glow_color;
		pawn->GetGlowProperty().m_iGlowType() = 3;
	}

	/*if (g_variables.scope_viewmodel) {
		CPlayer_WeaponServices* pWeaponServices = sdk::local_pawn->m_pWeaponServices();
		if (pWeaponServices) {
			C_CSWeaponBase* pActiveWeapon = pWeaponServices->m_hActiveWeapon().Get<C_CSWeaponBase>();
			if (pActiveWeapon)
				pActiveWeapon->GetWeaponVData()->m_bHideViewModelWhenZoomed() = false;
		}
	}*/


	// nightmode
	if (C_PostProcessingVolume* post_processing = sdk::local_pawn->m_pCameraServices()->m_hActivePostProcessingVolume().Get<C_PostProcessingVolume>()) {
		if (post_processing)
		{
			float amount = (101 - g_variables.nightmode_strength) / 100.f;

			post_processing->m_bExposureControl() = g_variables.nightmode;
			post_processing->m_flMinExposure() = amount;
			post_processing->m_flMaxExposure() = amount;
		}
	}
}

material_t* esp::create_material(const std::string_view material_name, bool ignore_z, bool translucent, bool blend)
{
	std::array< byte*, 0x200 > buffer{ };

	const struct set_material_data_t {
		inline explicit constexpr set_material_data_t(void* data) : m_data(data) {}

		__forceinline void init_shader_type(const std::string_view shader_name) const {
			// particles.dll; E8 ?? ?? ?? ?? 48 8D B7 ?? ?? ?? ?? + 1
			static const auto fn = signature::find("particles.dll", "E8 ?? ?? ?? ?? 48 8D B7 ?? ?? ?? ??").add(0x1).rel32().get<void(__fastcall*)(void*, key_string_t, const char*, int)>();

			constexpr std::uintptr_t shader_key = 0x162C1777u;
			const key_string_t shader("shader", shader_key);
			return fn(m_data, shader, shader_name.data(), 0x18);
		}

		__forceinline void init_functions(const std::string_view functions_name, const int value = 1) const {
			// particles.dll; 48 89 5C 24 08 48 89 6C 24 10 56 57 41 54
			static const auto fn = signature::find("particles.dll", "48 89 5C 24 08 48 89 6C 24 10 56 57 41 54").get<void(__fastcall*)(void*, key_string_t, int, int)>();

			const key_string_t functions(functions_name.data(), 0u);
			return fn(m_data, functions, value, 0x12);
		}

		[[nodiscard]] __forceinline auto get_data() const noexcept { return this->m_data; }
	private:
		void* m_data{ };
	} material(buffer.data() + 0x50);

	material_t** material_prototype;
	interfaces::g_material_system->find_material(&material_prototype, "materials/dev/primary_white.vmat");
	interfaces::g_material_system->set_create_data_by_material(material.get_data(), &material_prototype);

	material.init_shader_type("csgo_unlitgeneric.vfx");
	material.init_functions("F_BLEND_MODE", blend);
	material.init_functions("F_TRANSLUCENT", translucent);
	material.init_functions("F_DISABLE_Z_BUFFERING", ignore_z);

	material_t** custom_material;
	interfaces::g_material_system->create_material(&custom_material, material_name, material.get_data());

	return *custom_material;
}

