#pragma once

#include "3rdparty/lvgl/lvgl.h"

class DevConsoleInput {
   private:
	lv_obj_t* _inputField;
	lv_obj_t* _runButton;
	lv_obj_t* _runButtonLabel;

   public:
	void Init(lv_obj_t* stripe) {
		_inputField = lv_textarea_create(stripe);
		lv_group_add_obj(lv_group_get_default(), _inputField);
		lv_obj_add_flag(_inputField, LV_OBJ_FLAG_EVENT_BUBBLE);
		lv_textarea_set_one_line(_inputField, true);
		lv_obj_set_flex_grow(_inputField, 1);
		lv_textarea_set_placeholder_text(_inputField, "Enter command here (type \"help\" for help)");

		_runButton = lv_button_create(stripe);
		lv_group_add_obj(lv_group_get_default(), _runButton);
		lv_obj_add_flag(_runButton, LV_OBJ_FLAG_EVENT_BUBBLE);

		_runButtonLabel = lv_label_create(_runButton);
		lv_label_set_text(_runButtonLabel, "Run");
	}
};