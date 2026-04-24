#pragma once

#include <string>

#include "engine/mods/ModManager.hpp"
#include "engine/ui/MenuTransparentBG.hpp"
#include "engine/world/WorldManager.hpp"
#include "game/ui/title/ModSelectButton.hpp"
#include "lvgl/lvgl.h"

// FIXME MASSIVE BODGE ALRERT !!!! MUST do cleanup, abstraction
class ModSelectMenu : public MenuTransparentBG {
	static void KeyPressedCallback(lv_event_t* event) {
		if (lv_indev_get_key(lv_indev_active()) == LV_KEY_ESC) {
			UIManager::Pop();
		}
	}

	static void ModSelectedCallback(lv_event_t* event) {
		int index = *(int*)lv_event_get_user_data(event);
		std::string path = ModManager::GetLevelPath(index);
		WorldManager::LoadLevel(path);
		UIManager::ClearStack();
	}

   protected:
	lv_obj_t* _panel;
	std::vector<ModSelectButton> _buttons;

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_size(_panel, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_center(_panel);

		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_gridnav_add(_panel, (lv_gridnav_ctrl_t)(LV_GRIDNAV_CTRL_ROLLOVER | LV_GRIDNAV_CTRL_HORIZONTAL_MOVE_ONLY));

		lv_group_add_obj(UIManager::GetMainGroup(), _panel);

		int modCount = ModManager::GetLevelCount();
		vector<std::string> levels = ModManager::GetLevelNames();

		_buttons.reserve(modCount);

		for (int i = 0; i < modCount; i++) {
			_buttons.emplace_back(_panel, i, levels[i], ModSelectedCallback);
		}

		lv_obj_add_event_cb(_panel, KeyPressedCallback, LV_EVENT_KEY, nullptr);
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_panel);
	}
};