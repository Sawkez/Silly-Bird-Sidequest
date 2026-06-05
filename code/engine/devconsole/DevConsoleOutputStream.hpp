#pragma once

#include <SDL3/SDL.h>

#include <iostream>
#include <string>

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"

class DevConsoleOutputStream {
   private:
	IDevConsoleMenu* _menu = nullptr;
	std::ostream& _stream;
	lv_color_t _color;
	std::string _text = "";

   public:
	DevConsoleOutputStream(lv_color_t color, std::ostream& stream) : _color(color), _stream(stream) {}

	void Init(IDevConsoleMenu* menu) { _menu = menu; }

	DevConsoleOutputStream& operator<<(const std::string& str) {
		_text += str;
		_stream << str;
		return *this;
	}

	DevConsoleOutputStream& operator<<(const char* str) {
		_text += str;
		_stream << str;
		return *this;
	}

	template <typename Type>
	DevConsoleOutputStream& operator<<(const Type& obj) {
		_text += std::to_string(obj);
		_stream << obj;
		return *this;
	}

	DevConsoleOutputStream& operator<<(const DevConsoleNewline& newline) {
		_menu->PrintLine(_text, _color);
		_stream << std::endl;
		_text.clear();
		return *this;
	}
};