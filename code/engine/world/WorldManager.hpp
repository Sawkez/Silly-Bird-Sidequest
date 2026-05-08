#pragma once

#include <memory>
#include <string>

#include "engine/GameState.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/save/SaveData.hpp"
#include "engine/save/SaveManager.hpp"
#include "engine/world/Level.hpp"
#include "yyjson.h"

class WorldManager {
   private:
	static inline std::unique_ptr<Level> _level;

   public:
	static void LoadLevel(const std::string& path, bool overrideProperties = false, int checkpoint = 0, int upgrades = 0, int room = 0) {
		yyjson_doc* jsonDoc = ResourceManager::LoadJson(path + "/level.json");
		yyjson_val* jsonRoot = yyjson_doc_get_root(jsonDoc);

		if (!overrideProperties) {
			upgrades = yyjson_get_int(yyjson_obj_get(jsonRoot, "starting_upgrades"));
			room = yyjson_get_int(yyjson_obj_get(jsonRoot, "starting_room"));
		}

		_level = std::make_unique<Level>(path, GameState::GetMainRenderer(), GameState::GetInput(), GameState::GetMainWindow(), room, upgrades);

		if (overrideProperties) {
			_level.get()->MovePlayerToCheckpoint(checkpoint);
		} else {
			_level.get()->GetPlayer().position.x = yyjson_get_num(yyjson_obj_get(jsonRoot, "player_x"));
			_level.get()->GetPlayer().position.y = yyjson_get_num(yyjson_obj_get(jsonRoot, "player_y"));
			_level.get()->UpdateCheckpoint();
		}

		SaveManager::instance->saveData.SetPath(path);
	}

	static void LoadLevel(const SaveData& save) { LoadLevel(save.modPath, true, save.checkpoint, save.upgrades, save.room); }

	static Level& GetLevel() { return *_level; }
};