#pragma once

#include "lvgl/lvgl.h"

namespace Styles {

auto focus = lv_style_t();

void Init() {
	lv_style_init(&focus);
	lv_style_set_outline_width(&focus, lv_dpx(2));
	lv_style_set_outline_color(&focus, lv_palette_main(LV_PALETTE_BLUE));
	lv_style_set_outline_pad(&focus, lv_dpx(2));
	lv_style_set_size(&focus, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
}
}  // namespace Styles