#pragma once

#include "engine/ui/MenuTransparentBG.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class TestMenu : public MenuTransparentBG {
	lv_obj_t* _label;

   public:
	void Init() override {
		MenuTransparentBG::Init();

		_label = lv_label_create(_screen);
		lv_label_set_text(_label, "i eat bugs! hooray to Bug the Eater!");
		lv_obj_center(_label);
	}
};