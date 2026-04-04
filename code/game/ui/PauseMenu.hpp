#pragma once

#include "engine/input/UIInputManager.hpp"
#include "engine/ui/Menu.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class PauseMenu : public Menu {
   private:
	static inline const char* _buttonMap[] = {"unpause", "\n", "i", "eat", "bugs", "🧟", NULL};
	lv_obj_t* _buttons;

	static void ButtonPressedCallback(lv_event_t* event) {
		auto* screen = (lv_obj_t*)lv_event_get_user_data(event);
		if (screen != lv_screen_active()) return;

		lv_obj_t* buttons = lv_event_get_target_obj(event);
		uint32_t button = lv_buttonmatrix_get_selected_button(buttons);

		switch (button) {
			case 0:
				Unpause();
		}
	}

	static void KeyPressedCallback(lv_event_t* event) {
		auto* screen = (lv_obj_t*)lv_event_get_user_data(event);
		if (screen != lv_screen_active()) return;

		if (lv_event_get_key(event) == LV_KEY_ESC) Unpause();
	}

	static void Unpause() { UIManager::Hide(); }

   public:
	PauseMenu() : Menu(), _buttons(lv_buttonmatrix_create(_screen)) {
		lv_buttonmatrix_set_map(_buttons, _buttonMap);

		lv_buttonmatrix_set_button_ctrl_all(_buttons, LV_BUTTONMATRIX_CTRL_CLICK_TRIG);

		lv_obj_add_style(_buttons, &Styles::focus, LV_PART_ITEMS | LV_STATE_FOCUSED);
		lv_obj_center(_buttons);

		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);

		lv_group_add_obj(UIInputManager::GetMainGroup(), _screen);

		lv_obj_add_event_cb(_buttons, ButtonPressedCallback, LV_EVENT_VALUE_CHANGED, _screen);
		lv_obj_add_event_cb(_buttons, KeyPressedCallback, LV_EVENT_KEY, _screen);
	}

	void Activate() override {
		Menu::Activate();
		lv_group_focus_obj(_buttons);
		lv_buttonmatrix_set_selected_button(_buttons, 0);
	}
};