#pragma once

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"
#include "engine/ui/DevConsoleMenu.hpp"

namespace DevConsole {
DevConsoleMenu menu;
DevConsoleNewline endl;
DevConsoleOutputStream msg(menu, lv_color_black());
DevConsoleOutputStream err(menu, lv_color_hex(0x880000));

void Init() { menu.Init(); }
}  // namespace DevConsole

namespace dc = DevConsole;