#pragma once

#include <string>

#include "engine/ResourceManager.hpp"
#include "yyjson.h"

class Mod {
   private:
	std::string _path;
	std::string _name;

   public:
	Mod(const std::string& path, yyjson_val* json) : _path(path), _name(yyjson_get_str(yyjson_obj_get(json, "name"))) {}

	Mod(const std::string& path, yyjson_doc* doc) : Mod(path, yyjson_doc_get_root(doc)) { yyjson_doc_free(doc); }
	Mod(const std::string& path) : Mod(path, ResourceManager::LoadJson(path + "/mod.json")) {}

	const std::string& GetName() const { return _name; }
	const std::string& GetPath() const { return _path; }
};