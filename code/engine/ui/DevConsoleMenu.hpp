#pragma once

#include <deque>
#include <string>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsoleInput.hpp"
#include "engine/devconsole/DevConsoleMessage.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"
#include "engine/ui/MenuTransparentBG.hpp"
#include "engine/ui/UIManager.hpp"

class DevConsoleMenu : public MenuTransparentBG, public IDevConsoleMenu {
	lv_obj_t* _panel;
	std::deque<DevConsoleMessage> _messages;
	bool _active = false;

	lv_obj_t* _backButton;
	lv_obj_t* _backButtonLabel;
	lv_obj_t* _controlStripe;
#ifdef PLATFORM_HAS_STRING_COMMANDS
	DevConsoleInput _input;
#endif

	static void KeyPressedCallback(lv_event_t* event) {
		if (lv_event_get_user_data(event) != lv_screen_active()) return;
		if (lv_indev_get_key(lv_indev_active()) == LV_KEY_ESC) {
			Unpause();
		}
	}

	static void Unpause() {
		UIManager::ClearStack();
		GameState::Unpause();
	}

	static void BackPressedCallback(lv_event_t* event) { Unpause(); }

   public:
	void Init() override {
		MenuTransparentBG::Init();

		lv_obj_set_layout(_screen, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_screen, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_style_pad_row(_screen, 0, 0);

		_panel = lv_obj_create(_screen);
		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_panel, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flag(_panel, LV_OBJ_FLAG_SCROLLABLE, true);
		lv_obj_set_size(_panel, lv_pct(100), lv_pct(50));
		lv_group_add_obj(lv_group_get_default(), _screen);
		lv_group_add_obj(lv_group_get_default(), _panel);
		lv_obj_add_flag(_panel, LV_OBJ_FLAG_EVENT_BUBBLE);

		lv_obj_add_event_cb(_screen, KeyPressedCallback, LV_EVENT_KEY, _screen);

		_controlStripe = lv_obj_create(_screen);
		lv_obj_add_flag(_controlStripe, LV_OBJ_FLAG_EVENT_BUBBLE);
		lv_group_add_obj(lv_group_get_default(), _controlStripe);
		lv_obj_set_size(_controlStripe, lv_pct(100), LV_SIZE_CONTENT);
		lv_obj_set_layout(_controlStripe, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_controlStripe, LV_FLEX_FLOW_ROW);

		_backButton = lv_button_create(_controlStripe);
		lv_obj_add_event_cb(_backButton, BackPressedCallback, LV_EVENT_CLICKED, nullptr);
		lv_obj_add_flag(_backButton, LV_OBJ_FLAG_EVENT_BUBBLE);

		_backButtonLabel = lv_label_create(_backButton);
		lv_label_set_text(_backButtonLabel, "Close");

#ifdef PLATFORM_HAS_STRING_COMMANDS
		_input.Init(_controlStripe);
#endif
	}

	void Activate() override {
		MenuTransparentBG::Activate();
		GameState::Pause();

		_active = true;

		for (auto& message : _messages) {
			message.CreateLabel(_panel);
		}

		_messages.back().ScrollIntoView();
		lv_group_focus_obj(_panel);
	}

	void Deactivate() override {
		MenuTransparentBG::Deactivate();
		_active = false;

		for (auto& message : _messages) {
			message.DestroyLabel();
		}

		lv_group_focus_obj(_panel);
	}

	void PrintLine(const std::string& text, const lv_color_t& color) override {
		bool lastMessageVisible = false;
		if (_active && !_messages.empty()) lastMessageVisible = _messages.back().IsVisible();

		if (_messages.size() >= PLATFORM_DEVCONSOLE_MAX_LINES) {
			_messages.pop_front();
		}

		_messages.emplace_back(text, color);

		if (_active) {
			_messages.back().CreateLabel(_panel);
			if (lastMessageVisible) _messages.back().ScrollIntoView();
		}
	}

	void Clear() override {
		for (auto& message : _messages) {
			message.DestroyLabel();
		}

		_messages.clear();
	}
};