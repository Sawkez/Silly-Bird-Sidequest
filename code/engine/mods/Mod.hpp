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

   protected:
	SDL_Storage* GetStorageFromPath(const std::string& path) {
		int length = path.length();

		if (length < MOD_EXTENSION_LENGTH) {
			dc::msg << "Loading mod " << path << " as folder!" << dc::endl;
			return SDL_OpenTitleStorage(path.c_str(), 0);
		}

		for (int i = 1; i < MOD_EXTENSION_LENGTH; i++) {
			if (path[length - i] != MOD_EXTENSION[MOD_EXTENSION_LENGTH - i - 1]) {
				dc::msg << "Loading mod " << path << " as folder!" << dc::endl;
				return SDL_OpenTitleStorage(path.c_str(), 0);
			}
		}

		dc::msg << "Loading mod " << path << " as archive!" << dc::endl;
		return ZipStorage::Open(path);
	}

   public:
	Mod(const std::string& path, SDL_Storage* storage, yyjson_val* json)
		: _path(path), _name(yyjson_get_str(yyjson_obj_get(json, "name"))), _storage(storage) {}

	Mod(const std::string& path, SDL_Storage* storage, yyjson_doc* doc) : Mod(path, storage, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}

	Mod(const std::string& path, SDL_Storage* storage)
		: Mod(path, storage, ResourceManager::LoadJson(storage, "mod.json")) {}

	Mod(const std::string& path) : Mod(path, GetStorageFromPath(path)) {}

	~Mod() { SDL_CloseStorage(_storage); }

	const std::string& GetPath() const { return _path; }

	SDL_Storage* GetStorage() { return _storage; }
};