#pragma once

#include "engine/ui/Menu.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class TestMenu : public Menu {
	lv_obj_t* _label;

   public:
	void Init() override {
		Menu::Init();

		_label = lv_label_create(_screen);
		lv_label_set_text(_label, "i eat bugs! hooray to Bug the Eater!");
		lv_obj_set_style_bg_opa(_screen, LV_OPA_TRANSP, LV_PART_MAIN);
		lv_obj_center(_label);
	}
};