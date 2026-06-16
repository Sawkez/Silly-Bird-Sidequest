#pragma once

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"

namespace DevConsole {
IDevConsoleMenu* menu;
DevConsoleNewline endl;
DevConsoleOutputStream msg(lv_color_black(), SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
DevConsoleOutputStream err(lv_color_hex(0x880000), SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_ERROR);

void Init(IDevConsoleMenu* consoleMenu) {
	menu = consoleMenu;
	msg.Init(consoleMenu);
	err.Init(consoleMenu);
}

void Clear() { menu->Clear(); }

}  // namespace DevConsole

namespace dc = DevConsole;