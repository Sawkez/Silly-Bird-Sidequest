#pragma once

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"

namespace DevConsole {
DevConsoleNewline endl;
DevConsoleOutputStream msg(lv_color_black(), SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
DevConsoleOutputStream err(lv_color_hex(0x880000), SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);

void Init(IDevConsoleMenu* menu) {
	msg.Init(menu);
	err.Init(menu);
}

}  // namespace DevConsole

namespace dc = DevConsole;