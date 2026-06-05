#pragma once

#include <deque>
#include <string>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/PlatformDefines.hpp"
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
		_panel = lv_obj_create(_screen);
		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_panel, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_flag(_panel, LV_OBJ_FLAG_SCROLLABLE, true);
		lv_obj_set_size(_panel, lv_pct(100), lv_pct(50));
		lv_group_add_obj(lv_group_get_default(), _screen);
		lv_group_add_obj(lv_group_get_default(), _panel);
		lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 0);
		lv_obj_add_flag(_panel, LV_OBJ_FLAG_EVENT_BUBBLE);

		lv_obj_add_event_cb(_screen, KeyPressedCallback, LV_EVENT_KEY, _screen);

		_backButton = lv_button_create(_screen);
		lv_obj_add_event_cb(_backButton, BackPressedCallback, LV_EVENT_CLICKED, nullptr);
		lv_obj_set_align(_backButton, LV_ALIGN_BOTTOM_LEFT);
		lv_obj_add_flag(_backButton, LV_OBJ_FLAG_EVENT_BUBBLE);

		_backButtonLabel = lv_label_create(_backButton);
		lv_label_set_text(_backButtonLabel, "Close console");
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
	}

	void PrintLine(const std::string& text, const lv_color_t& color) override {
		if (_messages.size() >= PLATFORM_DEVCONSOLE_MAX_LINES) {
			_messages.pop_front();
		}

		_messages.emplace_back(text, color);

		if (_active) _messages.back().CreateLabel(_panel);
	}
};