#pragma once

#include <string>
#include <vector>

#include "engine/mods/Mod.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ZipStorage.hpp"
#include "yyjson.h"

class ResourceMod : public Mod {
   private:
	std::vector<std::string> _tileSources;

   public:
	ResourceMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_val* json)
		: Mod(modStorage, modPath, json) {
		int idx, max;
		yyjson_val* sources = yyjson_obj_get(json, "tilesheet_sources");
		yyjson_val* source;

		_tileSources.reserve(yyjson_arr_size(sources));

		yyjson_arr_foreach(sources, idx, max, source) { _tileSources.emplace_back(yyjson_get_str(source)); }
	}

	ResourceMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_doc* jsonDoc)
		: ResourceMod(modStorage, modPath, yyjson_doc_get_root(jsonDoc)) {
		yyjson_doc_free(jsonDoc);
	}

	ResourceMod(SDL_Storage* modStorage, const std::string& modPath)
		: ResourceMod(modStorage, modPath, ResourceManager::LoadJson(modStorage, "mod.json")) {}

	std::string GetTileSourcePath(uint8_t sourceID) const { return "tiles/fg/" + _tileSources[sourceID]; }
};