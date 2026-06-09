#pragma once

#include "engine/devconsole/DevConsoleCommand.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMisc.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMod.hpp"

namespace DevConsoleCommandRegister {

void Init() {
	auto reg = DevConsoleCommandManager::RegisterCommand;

	reg("help", ConsoleCommandsMisc::Help, 0, 0, "List available commands");
	reg("loadModLevelIndex", ConsoleCommandsMod::loadModLevelIndex, DevConsoleCommand::FLAG_UNSAFE, 1,
		"[mod_path] [level_index] - Load mod level by index");
}

}  // namespace DevConsoleCommandRegister