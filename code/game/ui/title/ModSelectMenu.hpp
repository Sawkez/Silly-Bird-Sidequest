#pragma once

#include <filesystem>
#include <vector>

#include "engine/ui/MenuTransparentBG.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/title/ModSelectButton.hpp"
#include "lvgl/lvgl.h"

class ModSelectMenu : public MenuTransparentBG {
	lv_obj_t* _panel;
	std::vector<ModSelectButton> _mods;

	static void ModPressedCallback(lv_event_t* event) {
		std::cout << "Selected mod " << (char*)lv_event_get_user_data(event) << std::endl;
		UIManager::Hide();
		WorldManager::LoadLevel((char*)lv_event_get_user_data(event));
	}

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_size(_panel, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_center(_panel);

		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);

		for (auto& entry : std::filesystem::directory_iterator("mods")) {
			_mods.emplace_back(_panel, entry.path().string(), ModPressedCallback);
		}
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_panel);
	}
};