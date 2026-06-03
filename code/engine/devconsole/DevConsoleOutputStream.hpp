#pragma once

#include <string>

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/ui/DevConsoleMenu.hpp"

class DevConsoleOutputStream {
   private:
	DevConsoleMenu& _menu;
	lv_color_t _color;
	std::string _text = "";

   public:
	DevConsoleOutputStream(DevConsoleMenu& menu, lv_color_t color) : _menu(menu), _color(color) {}

	DevConsoleOutputStream& operator<<(const std::string& str) {
		_text += str;
		return *this;
	}

	DevConsoleOutputStream& operator<<(const char* str) {
		_text += str;
		return *this;
	}

	template <typename Type>
	DevConsoleOutputStream& operator<<(const Type& obj) {
		_text += std::to_string(obj);
		return *this;
	}

	DevConsoleOutputStream& operator<<(const DevConsoleNewline& newline) {
		_menu.PrintLine(_text, _color);
		_text = "";
		return *this;
	}
};