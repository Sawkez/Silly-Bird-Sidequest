#pragma once

#include <string>
#include <vector>

#include "engine/ResourceManager.hpp"
#include "engine/mods/Mod.hpp"
#include "engine/mods/SubmodInfo.hpp"
#include "yyjson.h"

class SkinMod : public Mod {
   private:
	std::vector<SubmodInfo> _skins;
	int _skinCount;

   public:
	SkinMod(const std::string& path, yyjson_val* json) : Mod(path, json) {
		yyjson_val* skinsJson = yyjson_obj_get(json, "skins");

		_skinCount = yyjson_arr_size(skinsJson);
		_skins.reserve(_skinCount);

		int idx;
		yyjson_val* skin;
		yyjson_arr_foreach(skinsJson, idx, _skinCount, skin) { _skins.emplace_back(skin); }
	}

	SkinMod(const std::string& path, yyjson_doc* doc) : SkinMod(path, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}
	SkinMod(const std::string& path) : SkinMod(path, ResourceManager::LoadJson(path + "/mod.json")) {}

	std::string GetSkinPath(int index) { return GetPath() + "/skins/" + _skins[index].GetPath(); }
	int GetSkinCount() const { return _skinCount; }
	const std::vector<SubmodInfo>& GetSkins() const { return _skins; }
};