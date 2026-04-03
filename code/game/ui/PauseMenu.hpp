#pragma once

#include "engine/input/UIInputManager.hpp"
#include "engine/ui/Menu.hpp"
#include "engine/ui/UIManager.hpp"
#include "lvgl/lvgl.h"

class PauseMenu : public Menu {
   private:
	static inline const char* _buttonMap[] = {"unpause", "\n", "i", "eat", "bugs", "🧟", NULL};
	lv_obj_t* _buttons;

	static void ButtonPressedCallback(lv_event_t* event) {
		lv_obj_t* buttons = lv_event_get_target_obj(event);
		uint32_t button = lv_buttonmatrix_get_selected_button(buttons);

		switch (button) {
			case 0:
				UIManager::Hide();
		}
	}

   public:
	PauseMenu() : Menu(), _buttons(lv_buttonmatrix_create(_screen)) {
		lv_buttonmatrix_set_map(_buttons, _buttonMap);

		lv_obj_center(_buttons);

		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);

		lv_group_add_obj(UIInputManager::GetMainGroup(), _screen);

		lv_obj_add_event_cb(_buttons, ButtonPressedCallback, LV_EVENT_VALUE_CHANGED, this);
	}
};