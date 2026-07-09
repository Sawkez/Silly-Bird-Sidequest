#pragma once

#include <string>

#include "engine/ResourceManager.hpp"
#include "yyjson.h"

class Mod {
   private:
	std::string _path;
	std::string _name;
	SDL_Storage* _storage;

   public:
	Mod(const std::string& path, SDL_Storage* storage, yyjson_val* json)
		: _path(path), _name(yyjson_get_str(yyjson_obj_get(json, "name"))), _storage(storage) {}

	Mod(const std::string& path, SDL_Storage* storage, yyjson_doc* doc) : Mod(path, yyjson_doc_get_root(doc)) {
		yyjson_doc_free(doc);
	}

	Mod(const std::string& path, SDL_Storage* storage)
		: Mod(path, storage, ResourceManager::LoadJson(storage, "mod.json")) {}

	Mod(const std::string& path) : Mod(path, SDL_OpenTitleStorage(path.c_str(), 0)) {}

	~Mod() { SDL_CloseStorage(_storage); }

	const std::string& GetPath() const { return _path; }

	SDL_Storage* GetStorage() { return _storage; }
};