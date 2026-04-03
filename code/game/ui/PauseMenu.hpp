#pragma once

#include "engine/ui/Menu.hpp"
#include "lvgl/lvgl.h"

class PauseMenu : public Menu {
   private:
	lv_obj_t* _buttons;

   public:
	PauseMenu() : Menu(), _buttons(lv_buttonmatrix_create(_screen)) {
		static const char* map[] = {"i", "eat", "bugs", NULL};
		lv_buttonmatrix_set_map(_buttons, map);

		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);
	}
};