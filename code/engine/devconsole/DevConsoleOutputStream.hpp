#pragma once

#include <SDL3/SDL.h>

#include <charconv>
#include <iostream>
#include <string>

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"

class DevConsoleOutputStream {
   private:
	static inline const int TEXT_SIZE = 512;

	IDevConsoleMenu* _menu = nullptr;
	lv_color_t _color;
	std::string _text = "";
	SDL_LogCategory _category;
	SDL_LogPriority _priority;

   public:
	DevConsoleOutputStream(lv_color_t color, SDL_LogCategory category, SDL_LogPriority priority)
		: _color(color), _category(category), _priority(priority) {
		_text.reserve(TEXT_SIZE);
	}

	void Init(IDevConsoleMenu* menu) { _menu = menu; }

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
		char buf[32];
		auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), obj);
		_text.append(buf, ptr);
		return *this;
	}

	DevConsoleOutputStream& operator<<(const DevConsoleNewline& newline) {
		SDL_LogMessage(_category, _priority, "%s", _text.data());
		if (_menu != nullptr) _menu->PrintLine(_text, _color);
		_text.clear();
		return *this;
	}
};