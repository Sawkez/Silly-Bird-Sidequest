#pragma once

#include <string>

#include "engine/ui/UIManager.hpp"
#include "game/ui/Styles.hpp"
#include "lvgl/lvgl.h"

class DirectorySelectButton {
   private:
	lv_obj_t* _button;
	lv_obj_t* _label;
	int _index;

   public:
	DirectorySelectButton(lv_obj_t* parent, const std::string& label, int index, lv_event_cb_t callback)
		: _button(lv_button_create(parent)), _label(lv_label_create(_button)), _index(index) {
		lv_label_set_text(_label, label.data());
		lv_group_remove_obj(_button);

		lv_obj_add_event_cb(_button, callback, LV_EVENT_CLICKED, &_index);

		lv_obj_add_style(_button, Styles::GetFocusStyle(), LV_PART_MAIN | LV_STATE_FOCUSED);
	}

	~DirectorySelectButton() { lv_obj_delete_async(_button); }

	lv_obj_t* GetButton() { return _button; }

	void MoveToIndex(int index) { lv_obj_move_to_index(_button, index); }
};