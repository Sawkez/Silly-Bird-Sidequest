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
	LevelMod(const std::string& path, yyjson_val* json) : ResourceMod(path, json) {
		yyjson_val* levelsJson = yyjson_obj_get(json, "levels");

		_levelCount = yyjson_arr_size(levelsJson);
		_levels.reserve(_levelCount);

		int idx, max;
		yyjson_val* level;
		yyjson_arr_foreach(levelsJson, idx, _levelCount, level) { _levels.emplace_back(level); }
	}

	LevelMod(const std::string& path, yyjson_doc* doc) : LevelMod(path, yyjson_doc_get_root(doc)) { yyjson_doc_free(doc); }
	LevelMod(const std::string& path) : LevelMod(path, ResourceManager::LoadJson(path + "/mod.json")) {}

	const std::string& GetLevelPath(int index) { return GetPath() + _levels[index].GetPath(); }
	int GetLevelCount() const { return _levelCount; }
	const std::vector<ModLevelInfo>& GetLevels() const { return _levels; }
};