#pragma once

#include "engine/devconsole/DevConsoleCommand.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMisc.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMod.hpp"
#include "engine/devconsole/commands/ConsoleCommandsPlayer.hpp"

namespace DevConsoleCommandRegister {

void Init() {
	auto reg = DevConsoleCommandManager::RegisterCommand;

	reg("help", ConsoleCommandsMisc::help, 0, 0, "List available commands");
	reg("clear", ConsoleCommandsMisc::clear, 0, 1, "Clear console messages");
	reg("loadModLevelIndex", ConsoleCommandsMod::loadModLevelIndex, DevConsoleCommand::FLAG_UNSAFE, 2,
		"[mod_path] [level_index] - Load mod level by index");
	reg("giveUpgrades", ConsoleCommandsPlayer::giveUpgrade, DevConsoleCommand::FLAG_CHEAT, 3,
		"Give one or more upgrades to player");
}

}  // namespace DevConsoleCommandRegister