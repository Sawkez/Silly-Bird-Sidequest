#pragma once

#include <string>

#include "3rdparty/lvgl/lvgl.h"

class DevConsoleMessage {
   private:
	std::string _text;
	lv_color_t _color;
	lv_obj_t* _label = nullptr;

   public:
	DevConsoleMessage(const std::string& text, const lv_color_t& color) : _text(text), _color(color) {}

	DevConsoleMessage(const DevConsoleMessage&) = delete;
	DevConsoleMessage& operator=(const DevConsoleMessage&) = delete;

	DevConsoleMessage(DevConsoleMessage&& other) noexcept : _text(std::move(other._text)), _color(other._color), _label(other._label) {
		other._label = nullptr;
	}

	DevConsoleMessage& operator=(DevConsoleMessage&& other) noexcept {
		if (this != &other) {
			if (_label) lv_obj_delete_async(_label);
			_text = std::move(other._text);
			_color = other._color;
			_label = other._label;
			other._label = nullptr;
		}

		return *this;
	}

	void CreateLabel(lv_obj_t* parent) {
		if (_label != nullptr) return;
		_label = lv_label_create(parent);
		lv_obj_set_style_text_color(_label, _color, 0);
		lv_label_set_text(_label, _text.data());
	}

	void DestroyLabel() {
		if (_label == nullptr) return;
		lv_obj_delete_async(_label);
		_label = nullptr;
	}

	void ScrollIntoView() { lv_obj_scroll_to_view(_label, LV_ANIM_OFF); }

	~DevConsoleMessage() { DestroyLabel(); }
};