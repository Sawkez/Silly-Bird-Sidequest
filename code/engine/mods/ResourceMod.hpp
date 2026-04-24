#pragma once

#include <string>
#include <vector>

#include "engine/ResourceManager.hpp"
#include "engine/mods/Mod.hpp"
#include "yyjson.h"

class ResourceMod : public Mod {
   private:
	std::vector<std::string> _tileSources;

   public:
	ResourceMod(const std::string& path, yyjson_val* json) : Mod(path, json) {
		int idx, max;
		yyjson_val* sources = yyjson_obj_get(json, "tilesheet_sources");
		yyjson_val* source;

		_tileSources.reserve(yyjson_arr_size(sources));

		yyjson_arr_foreach(sources, idx, max, source) { _tileSources.emplace_back(yyjson_get_str(source)); }
	}

	ResourceMod(const std::string& path, yyjson_doc* doc) : ResourceMod(path, yyjson_doc_get_root(doc)) { yyjson_doc_free(doc); }
	ResourceMod(const std::string& path) : ResourceMod(path, ResourceManager::LoadJson(path + "/mod.json")) {}
};