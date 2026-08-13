#pragma once

#include <string>
#include <vector>

#include "engine/mods/ModManager.hpp"
#include "engine/world/WorldManager.hpp"

namespace ConsoleCommandsMod {

void loadCurrentModLevel(const std::vector<std::string>& args) {
	if (args[0] == "index") {
		WorldManager::LoadLevel(stoi(args[1]));
	} else if (args[0] == "path") {
		WorldManager::LoadLevel(ModManager::GetLevelIndexFromPath(args[1]));
	} else {
		dc::err << "1st argument should be \"index\" or \"path\"" << dc::endl;
	}
}

void loadModLevel(const std::vector<std::string>& args) {
	if (args[1] == "index") {
		ModManager::LoadLevelModFromFile(args[0]);
		WorldManager::LoadLevel(stoi(args[2]));

	} else if (args[1] == "path") {
		ModManager::LoadLevelModFromFile(args[0]);
		WorldManager::LoadLevel(ModManager::GetLevelIndexFromPath(args[2]));

	} else {
		dc::err << "2nd argument should be \"index\" or \"path\"" << dc::endl;
	}
}

void loadBuiltinLevel(const std::vector<std::string>& args) {
	if (args[0] == "index") {
		ModManager::LoadLevelModFromFolder(ResourceManager::gameData, "content/sidequest.sbsq");
		WorldManager::LoadLevel(stoi(args[1]));
	} else if (args[0] == "path") {
		ModManager::LoadLevelModFromFolder(ResourceManager::gameData, "content/sidequest.sbsq");
		WorldManager::LoadLevel(ModManager::GetLevelIndexFromPath(args[1]));
	} else {
		dc::err << "1st argument should be \"index\" or \"path\"" << dc::endl;
	}
}

void loadCurrentModSkin(const std::vector<std::string>& args) {
	if (args[0] == "index") {
		ModManager::LoadSkin(GameState::GetMainRenderer(), stoi(args[1]));
	} else if (args[0] == "path") {
		ModManager::LoadSkin(GameState::GetMainRenderer(), ModManager::GetSkinIndexFromPath(args[1]));
	} else {
		dc::err << "1st argument should be \"index\" or \"path\"" << dc::endl;
	}
}

void loadModSkin(const std::vector<std::string>& args) {
	if (args[1] == "index") {
		ModManager::LoadSkinModFromFile(args[0]);
		ModManager::LoadSkin(GameState::GetMainRenderer(), stoi(args[2]));
	} else if (args[1] == "path") {
		ModManager::LoadSkinModFromFile(args[0]);
		ModManager::LoadSkin(GameState::GetMainRenderer(), ModManager::GetSkinIndexFromPath(args[2]));
	} else {
		dc::err << "2nd argument should be \"index\" or \"path\"" << dc::endl;
	}
}

void loadBuiltinSkin(const std::vector<std::string>& args) {
	if (args[0] == "index") {
		ModManager::LoadSkinModFromFolder(ResourceManager::gameData, "content/sidequest.sbsq");
		ModManager::LoadSkin(GameState::GetMainRenderer(), stoi(args[1]));
	} else if (args[0] == "path") {
		ModManager::LoadSkinModFromFolder(ResourceManager::gameData, "content/sidequest.sbsq");
		ModManager::LoadSkin(GameState::GetMainRenderer(), ModManager::GetSkinIndexFromPath(args[1]));
	} else {
		dc::err << "1st argument should be \"index\" or \"path\"" << dc::endl;
	}
}
}  // namespace ConsoleCommandsMod