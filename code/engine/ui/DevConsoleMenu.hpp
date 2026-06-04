#pragma once

#include <deque>
#include <string>

#include "3rdparty/lvgl/lvgl.h"
#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsoleMessage.hpp"
#include "engine/ui/MenuTransparentBG.hpp"

class DevConsoleMenu : public MenuTransparentBG {
	lv_obj_t* _panel;
	std::deque<DevConsoleMessage> _messages;
	bool _active = false;

   public:
	void Init() override {
		MenuTransparentBG::Init();
		_panel = lv_obj_create(_screen);
		lv_obj_set_layout(_panel, LV_LAYOUT_FLEX);
		lv_obj_set_flex_flow(_panel, LV_FLEX_FLOW_COLUMN);
		lv_obj_set_size(_panel, lv_pct(100), lv_pct(50));
		lv_obj_align(_panel, LV_ALIGN_TOP_MID, 0, 0);
	}

	void Activate() override {
		MenuTransparentBG::Activate();
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

	void PrintLine(const std::string& text, const lv_color_t& color) {
		if (_messages.size() >= PLATFORM_DEVCONSOLE_MAX_LINES) {
			_messages.pop_front();
			std::cout << SDL_GetTicks() << ": removed message" << std::endl;
		}

		_messages.emplace_back(text, color);

		if (_active) _messages.back().CreateLabel(_panel);
	}
};