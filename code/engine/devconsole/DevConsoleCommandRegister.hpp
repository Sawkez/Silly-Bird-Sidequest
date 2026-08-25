#pragma once

#include "engine/devconsole/DevConsoleCommand.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMisc.hpp"
#include "engine/devconsole/commands/ConsoleCommandsMod.hpp"
#include "engine/devconsole/commands/ConsoleCommandsPlayer.hpp"
#include "engine/devconsole/commands/ConsoleCommandsVar.hpp"

namespace DevConsoleCommandRegister {

void Init() {
	auto reg = DevConsoleCommandManager::RegisterCommand;

	reg("help", ConsoleCommandsMisc::help, 0, CMD_MISC_HELP, "List available commands");
	reg("clear", ConsoleCommandsMisc::clear, 0, CMD_MISC_CLEAR, "Clear console messages");

	reg("loadCurrentModLevel", ConsoleCommandsMod::loadCurrentModLevel, 0, CMD_MOD_LEVEL_CURRENT,
		"index/path [level_index]/[level_path] - Load level from currently loaded level mod");
	reg("loadModLevel", ConsoleCommandsMod::loadModLevel, DC_FLAG_UNSAFE, CMD_MOD_LEVEL,
		"[mod_path] index/path [level_index]/[level_path] - Load level from mod file");
	reg("loadBuiltinLevel", ConsoleCommandsMod::loadBuiltinLevel, 0, CMD_MOD_LEVEL_BUILTIN,
		"index/path [level_index]/[level_path] - Load level from built-in campaign");
	reg("loadCurrentModSkin", ConsoleCommandsMod::loadModSkin, 0, CMD_MOD_SKIN_CURRENT,
		"index/path [skin_index]/[skin_path] - Load skin from currently loaded skin mod");
	reg("loadModSkin", ConsoleCommandsMod::loadModSkin, DC_FLAG_UNSAFE, CMD_MOD_SKIN,
		"[mod_path] index/path [skin_index]/[skin_path] - Load skin from mod file");
	reg("loadBuiltinSkin", ConsoleCommandsMod::loadBuiltinSkin, 0, CMD_MOD_SKIN_BUILTIN,
		"index/path [skin_index]/[skin_path] - Load skin from built-in mod");

	reg("giveUpgrades", ConsoleCommandsPlayer::giveUpgrade, DC_FLAG_CHEAT, CMD_PLAYER_GIVE_UPGRADES,
		"Give one or more upgrades to player");

	reg("set", ConsoleCommandsVar::set, DC_FLAG_NONE, CMD_VAR_SET, "Set console variable");
}

}  // namespace DevConsoleCommandRegister