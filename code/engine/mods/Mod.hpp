#pragma once

#include <iostream>
#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ZipStorage.hpp"
#include "yyjson.h"

class Mod {
   private:
	std::string _path;
	std::string _name;
	SDL_Storage* _storage;

	static inline const int MOD_EXTENSION_LENGTH = 6;
	static inline const char MOD_EXTENSION[MOD_EXTENSION_LENGTH] = ".sbsq";

   public:
	Mod(SDL_Storage* modStorage, const std::string& modPath, yyjson_val* json)
		: _name(yyjson_get_str(yyjson_obj_get(json, "name"))), _storage(modStorage) {}

	Mod(SDL_Storage* modStorage, const std::string& modPath, yyjson_doc* doc)
		: Mod(modStorage, modPath, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}

	Mod(SDL_Storage* modStorage, const std::string& modPath)
		: Mod(modStorage, modPath, ResourceManager::LoadJson(modStorage, "mod.json")) {}

	~Mod() { SDL_CloseStorage(_storage); }

	const std::string& GetPath() const { return _path; }

	SDL_Storage* GetStorage() { return _storage; }
};