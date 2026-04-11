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
	std::vector<std::string> _modPaths;

	static void ModPressedCallback(lv_event_t* event) {
		UIManager::Hide();
		std::string modPath = *(std::string*)lv_event_get_user_data(event);
		WorldManager::LoadLevel(modPath);
	}

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_size(_panel, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_center(_panel);

		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_gridnav_add(_panel, LV_GRIDNAV_CTRL_NONE);

		lv_group_add_obj(UIManager::GetMainGroup(), _panel);

		int modCount = 0;

		for (auto& entry : std::filesystem::directory_iterator("mods")) {
			_modPaths.push_back(entry.path().string());
			modCount++;
		}

		_mods.reserve(modCount);

		for (const auto& modPath : _modPaths) {
			_mods.emplace_back(_panel, modPath, ModPressedCallback);
		}
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_panel);
	}
};