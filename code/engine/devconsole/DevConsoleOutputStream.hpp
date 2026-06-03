#pragma once

#include <string>

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/ui/DevConsoleMenu.hpp"

class DevConsoleOutputStream {
   private:
	DevConsoleMenu& _menu;
	std::string _text = "";

   public:
	DevConsoleOutputStream(DevConsoleMenu& menu) : _menu(menu) {}

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
		_menu.PrintLine(_text);
		_text = "";
		return *this;
	}
};