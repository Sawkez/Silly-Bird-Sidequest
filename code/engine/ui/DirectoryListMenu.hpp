#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "engine/ui/DirectorySelectButton.hpp"
#include "engine/ui/MenuTransparentBG.hpp"
#include "engine/ui/UIManager.hpp"
#include "lvgl/lvgl.h"

class DirectoryListMenu : public MenuTransparentBG {
	static void KeyPressedCallback(lv_event_t* event) {
		if (lv_indev_get_key(lv_indev_active()) == LV_KEY_ESC) {
			UIManager::Pop();
		}
	}

   protected:
	lv_obj_t* _panel;
	std::vector<std::unique_ptr<DirectorySelectButton>> _buttons;
	static inline std::vector<std::string> _paths;

	virtual lv_event_cb_t GetSelectedCallback() const = 0;
	virtual void UpdatePaths() = 0;
	virtual std::string GetLabel(int index) const { return _paths[index]; }

	void AddButton(const std::string& label, int index) {
		_buttons.push_back(std::make_unique<DirectorySelectButton>(_panel, label, index, GetSelectedCallback()));
	}

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_size(_panel, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_center(_panel);

		lv_obj_set_scroll_dir(_panel, LV_DIR_HOR);

		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_gridnav_add(_panel, (lv_gridnav_ctrl_t)(LV_GRIDNAV_CTRL_ROLLOVER | LV_GRIDNAV_CTRL_HORIZONTAL_MOVE_ONLY));

		lv_group_add_obj(UIManager::GetMainGroup(), _panel);

		lv_obj_add_event_cb(_panel, KeyPressedCallback, LV_EVENT_KEY, nullptr);
	}

	void Activate() override {
		Menu::Activate();
		UpdatePaths();

		for (int i = 0; i < _paths.size(); i++) {
			AddButton(GetLabel(i), i);
		}

		lv_group_focus_obj(_panel);
	}

	void Deactivate() override {
		_paths.clear();
		_buttons.clear();
	}
};