#pragma once

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"
#include "engine/ui/DevConsoleMenu.hpp"

namespace DevConsole {
DevConsoleMenu menu;
DevConsoleNewline endl;
DevConsoleOutputStream msg(menu, lv_color_black(), std::cout);
DevConsoleOutputStream err(menu, lv_color_hex(0x880000), std::cerr);

void Init() { menu.Init(); }
}  // namespace DevConsole

namespace dc = DevConsole;