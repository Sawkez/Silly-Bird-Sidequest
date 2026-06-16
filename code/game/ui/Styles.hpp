#pragma once

#include "lvgl/lvgl.h"

class Styles {
   private:
	static inline auto _focus = lv_style_t();
	static inline const lv_font_t* _defaultFont;
	static inline const lv_font_t* _monoFont;

	static inline lv_theme_t* _defaultTheme;
	static inline lv_theme_t* _monoTheme;

   public:
	static void Init(lv_display_t* display, float contentScale) {
		if (contentScale > 1.75f) {
			_defaultFont = &lv_font_montserrat_28;
			_monoFont = &drafting_mono_28;
		} else if (contentScale > 0.75f) {
			_defaultFont = &lv_font_montserrat_14;
			_monoFont = &drafting_mono_14;
		} else {
			_defaultFont = &lv_font_montserrat_12;
			_monoFont = &drafting_mono_12;
		}

		_defaultTheme = lv_theme_default_init(display, lv_palette_main(LV_PALETTE_BLUE),
											  lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, _defaultFont);

		lv_display_set_theme(display, _defaultTheme);

		_monoTheme = lv_theme_mono_init(display, false, _monoFont);

		lv_obj_set_style_bg_opa(lv_display_get_layer_bottom(display), LV_OPA_TRANSP, 0);

		lv_style_init(&_focus);
		lv_style_set_outline_width(&_focus, lv_dpx(2));
		lv_style_set_outline_color(&_focus, lv_palette_main(LV_PALETTE_BLUE));
		lv_style_set_outline_pad(&_focus, lv_dpx(2));
		lv_style_set_size(&_focus, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
	}

	static lv_style_t* GetFocusStyle() { return &_focus; }

	static lv_theme_t* GetDefaultTheme() { return _defaultTheme; }
	static lv_theme_t* GetMonoTheme() { return _monoTheme; }
	static const lv_font_t* GetMonoFont() { return _monoFont; }
};