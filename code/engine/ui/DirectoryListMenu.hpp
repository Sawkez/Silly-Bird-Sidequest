#pragma once

#include <filesystem>
#include <vector>

#include "engine/ui/DirectorySelectButton.hpp"
#include "engine/ui/MenuTransparentBG.hpp"
#include "engine/ui/UIManager.hpp"
#include "lvgl/lvgl.h"

class DirectoryListMenu : public MenuTransparentBG {
	lv_obj_t* _panel;
	std::vector<DirectorySelectButton> _mods;
	std::vector<std::string> _modPaths;

	static void KeyPressedCallback(lv_event_t* event) {
		if (lv_indev_get_key(lv_indev_active()) == LV_KEY_ESC) {
			UIManager::Pop();
		}
	}

   protected:
	virtual lv_event_cb_t GetSelectedCallback() const = 0;

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_size(_panel, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_center(_panel);

		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_gridnav_add(_panel, (lv_gridnav_ctrl_t)(LV_GRIDNAV_CTRL_ROLLOVER | LV_GRIDNAV_CTRL_HORIZONTAL_MOVE_ONLY));

		lv_group_add_obj(UIManager::GetMainGroup(), _panel);

		int modCount = 0;

		for (auto& entry : std::filesystem::directory_iterator("mods")) {
			_modPaths.push_back(entry.path().string());
			modCount++;
		}

		_mods.reserve(modCount);

		for (const auto& modPath : _modPaths) {
			_mods.emplace_back(_panel, modPath, GetSelectedCallback());
		}

		lv_obj_add_event_cb(_panel, KeyPressedCallback, LV_EVENT_KEY, nullptr);
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_panel);
	}
};