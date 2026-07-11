#pragma once

#include "engine/ui/DevConsoleMenu.hpp"
#include "engine/ui/Menu.hpp"
#include "engine/ui/TestMenu.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/gameplay/PauseMenu.hpp"
#include "game/ui/title/LevelSelectMenu.hpp"
#include "game/ui/title/ModSelectMenu.hpp"
#include "game/ui/title/TitleScreenMenu.hpp"
#include "lvgl/lvgl.h"

namespace Menus {

auto test = TestMenu();
auto pause = PauseMenu();
auto title = TitleScreenMenu();
auto levels = LevelSelectMenu();
auto console = DevConsoleMenu();
auto mods = ModSelectMenu();

};	// namespace Menus

Menu* UIManager::_menus[UIManager::_MENU_COUNT] = {&Menus::test,   &Menus::pause,	&Menus::title,
												   &Menus::levels, &Menus::console, &Menus::mods};