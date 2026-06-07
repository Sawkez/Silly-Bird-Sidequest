#pragma once

#include <string>

#include "3rdparty/lvgl/lvgl.h"

class IDevConsoleMenu {
   public:
	virtual void PrintLine(const std::string& text, const lv_color_t& color) = 0;
};