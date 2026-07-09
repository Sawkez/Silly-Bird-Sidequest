#pragma once

#include <string>
#include <vector>

#include "engine/mods/Mod.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "yyjson.h"

class ResourceMod : public Mod {
   private:
	std::vector<std::string> _tileSources;

   public:
	ResourceMod(const std::string& path, SDL_Storage* storage, yyjson_val* json) : Mod(path, storage, json) {
		int idx, max;
		yyjson_val* sources = yyjson_obj_get(json, "tilesheet_sources");
		yyjson_val* source;

		_tileSources.reserve(yyjson_arr_size(sources));

		yyjson_arr_foreach(sources, idx, max, source) { _tileSources.emplace_back(yyjson_get_str(source)); }
	}

	ResourceMod(const std::string& path, SDL_Storage* storage, yyjson_doc* doc)
		: ResourceMod(path, storage, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}

	ResourceMod(const std::string& path, SDL_Storage* storage)
		: ResourceMod(path, storage, ResourceManager::LoadJson(storage, "mod.json")) {}

	ResourceMod(const std::string& path) : ResourceMod(path, GetStorageFromPath(path)) {}

	std::string GetTileSourcePath(uint8_t sourceID) const { return "tiles/fg/" + _tileSources[sourceID]; }
};