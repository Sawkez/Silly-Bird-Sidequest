#pragma once

#include "engine/ui/Menu.hpp"
#include "lvgl/lvgl.h"

class MenuTransparentBG : public Menu {
   public:
	void Init() override {
		Menu::Init();

		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);
	}
};