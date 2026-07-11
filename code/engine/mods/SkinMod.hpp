#pragma once

#include <string>
#include <vector>

#include "engine/mods/Mod.hpp"
#include "engine/mods/SubmodInfo.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "yyjson.h"

class SkinMod : public Mod {
   private:
	std::vector<SubmodInfo> _skins;
	int _skinCount;

   public:
	SkinMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_val* json) : Mod(modStorage, modPath, json) {
		yyjson_val* skinsJson = yyjson_obj_get(json, "skins");

		_skinCount = yyjson_arr_size(skinsJson);
		_skins.reserve(_skinCount);

		int idx;
		yyjson_val* skin;
		yyjson_arr_foreach(skinsJson, idx, _skinCount, skin) { _skins.emplace_back(skin); }
	}

	SkinMod(SDL_Storage* modStorage, const std::string& modPath, yyjson_doc* jsonDoc)
		: SkinMod(modStorage, modPath, yyjson_doc_get_root(jsonDoc)) {
		yyjson_doc_free(jsonDoc);
	}

	SkinMod(SDL_Storage* modStorage, const std::string& modPath)
		: SkinMod(modStorage, modPath, ResourceManager::LoadJson(modStorage, "mod.json")) {}

	std::string GetSkinPath(int index) { return "skins/" + _skins[index].GetPath(); }
	int GetSkinCount() const { return _skinCount; }
	const std::vector<SubmodInfo>& GetSkins() const { return _skins; }
};