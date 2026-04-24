#pragma once

#include <string>

#include "yyjson.h"

class ModLevelInfo {
   private:
	std::string _path;
	std::string _name;

   public:
	ModLevelInfo(yyjson_val* json) : _path(yyjson_get_str(yyjson_obj_get(json, "path"))), _name(yyjson_get_str(yyjson_obj_get(json, "name"))) {}
	const std::string& GetName() const { return _name; }
	const std::string& GetPath() const { return _path; }
};