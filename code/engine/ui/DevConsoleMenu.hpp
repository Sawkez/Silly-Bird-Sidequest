#pragma once

#include <string>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/ui/MenuTransparentBG.hpp"

class DevConsoleMenu : public MenuTransparentBG {
	lv_obj_t* _panel;

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_panel, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_size(_panel, lv_pct(100), lv_pct(50));
		lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 0);
	}

	void PrintLine(const std::string& text, const lv_color_t& color) {
		lv_obj_t* label = lv_label_create(_panel);
		lv_obj_set_style_text_color(label, color, 0);
		lv_label_set_text(label, text.data());
	}
};