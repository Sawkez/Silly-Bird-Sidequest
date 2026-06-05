#pragma once

#include "engine/devconsole/DevConsoleNewline.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"
#include "engine/ui/IDevConsoleMenu.hpp"

namespace DevConsole {
DevConsoleNewline endl;
DevConsoleOutputStream msg(lv_color_black(), std::cout);
DevConsoleOutputStream err(lv_color_hex(0x880000), std::cerr);

void Init(IDevConsoleMenu* menu) {
	msg.Init(menu);
	err.Init(menu);
}

}  // namespace DevConsole

namespace dc = DevConsole;