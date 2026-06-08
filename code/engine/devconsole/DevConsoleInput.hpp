#pragma once

#include <SDL3/SDL.h>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/GameState.hpp"
#include "engine/devconsole/DevConsole.hpp"

class DevConsoleInput {
   private:
	static inline lv_obj_t* _inputField;
	static inline lv_obj_t* _runButton;
	static inline lv_obj_t* _runButtonLabel;

	static void InputFieldPressedCallback(lv_event_t* event) {
		GameState::StartTextInput(HandleEvent);
		dc::msg << "Started text input!" << dc::endl;
	}

	static bool HandleEvent(const SDL_Event& event) {
		switch (event.type) {
			case SDL_EVENT_TEXT_INPUT:

				lv_textarea_add_text(_inputField, event.text.text);
				return true;

			default:
				return false;
		}
	}

   public:
	void Init(lv_obj_t* stripe) {
		_inputField = lv_textarea_create(stripe);
		lv_group_add_obj(lv_group_get_default(), _inputField);
		lv_obj_add_flag(_inputField, LV_OBJ_FLAG_EVENT_BUBBLE);
		lv_textarea_set_one_line(_inputField, true);
		lv_obj_set_flex_grow(_inputField, 1);
		lv_textarea_set_placeholder_text(_inputField, "Enter command here (type \"help\" for help)");

		lv_obj_add_event_cb(_inputField, InputFieldPressedCallback, LV_EVENT_FOCUSED, nullptr);

		_runButton = lv_button_create(stripe);
		lv_group_add_obj(lv_group_get_default(), _runButton);
		lv_obj_add_flag(_runButton, LV_OBJ_FLAG_EVENT_BUBBLE);

		_runButtonLabel = lv_label_create(_runButton);
		lv_label_set_text(_runButtonLabel, "Run");
	}
};