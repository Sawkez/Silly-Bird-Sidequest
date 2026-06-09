#pragma once

#include <string>
#include <vector>

#include "engine/mods/ModManager.hpp"
#include "engine/world/WorldManager.hpp"

namespace ConsoleCommandsMod {

void loadModLevelIndex(const std::vector<std::string>& args) {
	ModManager::LoadLevelMod(args[0]);
	int index = std::stoi(args[1]);
	WorldManager::LoadLevel(ModManager::GetLevelPath(index));
}
}  // namespace ConsoleCommandsMod