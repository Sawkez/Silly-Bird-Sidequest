#pragma once

#include "engine/ui/Menu.hpp"
#include "lvgl/lvgl.h"

class MenuTransparentBG : public Menu {
   public:
	void Init() override {
		Menu::Init();

		// lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);
		lv_obj_set_style_bg_color(_screen, lv_color_hex(0x000000), 0);
	}
};