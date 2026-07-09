#pragma once

#include <vector>

#include "engine/ResourceManager.hpp"
#include "engine/mods/ModLevelInfo.hpp"
#include "engine/mods/ResourceMod.hpp"
#include "yyjson.h"

class LevelMod : public ResourceMod {
   private:
	std::vector<ModLevelInfo> _levels;
	int _levelCount;

   public:
	LevelMod(const std::string& path, SDL_Storage* storage, yyjson_val* json) : ResourceMod(path, storage, json) {
		yyjson_val* levelsJson = yyjson_obj_get(json, "levels");

		_levelCount = yyjson_arr_size(levelsJson);
		_levels.reserve(_levelCount);

		int idx, max;
		yyjson_val* level;
		yyjson_arr_foreach(levelsJson, idx, _levelCount, level) { _levels.emplace_back(level); }
	}

	LevelMod(const std::string& path, SDL_Storage* storage, yyjson_doc* doc)
		: LevelMod(path, storage, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}

	LevelMod(const std::string& path, SDL_Storage* storage)
		: LevelMod(path, storage, ResourceManager::LoadJson(storage, "mod.json")) {}

	LevelMod(const std::string& path) : LevelMod(path, SDL_OpenTitleStorage(path.c_str(), 0)) {}

	std::string GetLevelPath(int index) { return "levels/" + _levels[index].GetPath() + "/"; }
	int GetLevelCount() const { return _levelCount; }
	const std::vector<ModLevelInfo>& GetLevels() const { return _levels; }
};