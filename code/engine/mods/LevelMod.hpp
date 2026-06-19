#pragma once

#include <vector>

#include "engine/mods/ResourceMod.hpp"
#include "engine/mods/SubmodInfo.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "yyjson.h"

class LevelMod : public ResourceMod {
   private:
	std::vector<SubmodInfo> _levels;
	int _levelCount;

   public:
	LevelMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_val* json)
		: ResourceMod(modStorage, modPath, json) {
		yyjson_val* levelsJson = yyjson_obj_get(json, "levels");

		_levelCount = yyjson_arr_size(levelsJson);
		_levels.reserve(_levelCount);

		int idx;
		yyjson_val* level;
		yyjson_arr_foreach(levelsJson, idx, _levelCount, level) { _levels.emplace_back(level); }
	}

	LevelMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_doc* jsonDoc)
		: LevelMod(modStorage, modPath, yyjson_doc_get_root(jsonDoc)) {
		yyjson_doc_free(jsonDoc);
	}

	LevelMod(SDL_Storage* modStorage, const std::string& modPath)
		: LevelMod(modStorage, modPath, ResourceManager::LoadJson(modStorage, "mod.json")) {}

	std::string GetLevelPath(int index) { return "levels/" + _levels[index].GetPath() + "/"; }
	int GetLevelCount() const { return _levelCount; }
	const std::vector<SubmodInfo>& GetLevels() const { return _levels; }
};