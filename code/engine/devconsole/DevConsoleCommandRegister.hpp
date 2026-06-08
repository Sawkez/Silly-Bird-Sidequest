#pragma once

#include "engine/devconsole/DevConsoleCommandManager.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMisc.hpp"

namespace DevConsoleCommandRegister {

void Init() {
	auto reg = DevConsoleCommandManager::RegisterCommand;

	reg("", ConsoleCommandsMisc::Typo, 0);
	reg("help", ConsoleCommandsMisc::Help, 1);
}

}  // namespace DevConsoleCommandRegister