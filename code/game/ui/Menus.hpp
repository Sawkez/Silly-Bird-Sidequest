#pragma once

#include "engine/ui/Menu.hpp"
#include "game/ui/PauseMenu.hpp"
#include "lvgl/lvgl.h"

namespace Menus {
enum MenuID { MENU_PAUSE, _MENU_COUNT };

Menu* pause;

void Init() { pause = new PauseMenu(); }
};	// namespace Menus