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

		if constexpr (std::is_floating_point_v<Type>) {
			int len = std::snprintf(buf, sizeof(buf), "%.4f", static_cast<double>(obj));
			if (len > 0) {
				_text.append(buf, static_cast<std::size_t>(len));
			}
		} else {
			auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), obj);
			if (ec == std::errc{}) {
				_text.append(buf, static_cast<std::size_t>(ptr - buf));
			}
		}
		return *this;
	}

	DevConsoleOutputStream& operator<<(const DevConsoleNewline& newline) {
		SDL_LogMessage(_category, _priority, "%s", _text.data());
		if (_menu != nullptr) _menu->PrintLine(_text, _color);
		_text.clear();
		return *this;
	}
};