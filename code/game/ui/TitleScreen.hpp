#pragma once

#include "engine/ui/Menu.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class TitleScreenMenu : public Menu {
	lv_obj_t* _buttons;
	static inline const char* _buttonMap[] = {"New game", "Load game", "\n", "Settings", "\n", "Quit", ""};

	static void ButtonPressedCallback(lv_event_t* event) {
		auto* screen = (lv_obj_t*)lv_event_get_user_data(event);
		if (screen != lv_screen_active()) return;

		lv_obj_t* buttons = lv_event_get_target_obj(event);
		uint32_t button = lv_buttonmatrix_get_selected_button(buttons);

		switch (button) {
			case 0:
				UIManager::Hide();
		}
	}

   public:
	TitleScreenMenu() : Menu() {
		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);

		_buttons = lv_buttonmatrix_create(_screen);
		lv_buttonmatrix_set_map(_buttons, _buttonMap);
		lv_obj_align(_buttons, LV_ALIGN_LEFT_MID, 0, 0);

		lv_obj_add_style(_buttons, &Styles::focus, LV_PART_ITEMS | LV_STATE_FOCUSED);

		lv_obj_add_event_cb(_buttons, ButtonPressedCallback, LV_EVENT_VALUE_CHANGED, _screen);
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_buttons);
		lv_buttonmatrix_set_selected_button(_buttons, 0);
	}

	void Deactivate() override { delete this; }
};