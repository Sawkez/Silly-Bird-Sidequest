#pragma once

#include "engine/devconsole/DevConsoleCommand.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMisc.hpp"

namespace DevConsoleCommandRegister {

void Init() {
	auto reg = DevConsoleCommandManager::RegisterCommand;

	reg("help", ConsoleCommandsMisc::Help, 0, 0);
}

}  // namespace DevConsoleCommandRegister