#pragma once

#include <string>

#include "engine/ui/UIManager.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class ModSelectButton {
   private:
	lv_obj_t* _button;
	lv_obj_t* _label;
	std::string _modPath;

   public:
	ModSelectButton(lv_obj_t* parent, std::string path, lv_event_cb_t callback)
		: _button(lv_button_create(parent)), _label(lv_label_create(_button)), _modPath(path) {
		lv_label_set_text(_label, _modPath.data());
		lv_group_remove_obj(_button);
		lv_obj_add_event_cb(_button, callback, LV_EVENT_CLICKED, _modPath.data());
		lv_obj_add_style(_button, &Styles::focus, LV_PART_MAIN | LV_STATE_FOCUSED);
	}

	lv_obj_t* GetButton() { return _button; }
};