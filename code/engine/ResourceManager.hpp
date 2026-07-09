#pragma once

#include <SDL3/SDL.h>

#include <string>

#include "yyjson.h"

namespace ResourceManager {

char* LoadText(SDL_Storage* storage, const std::string& path, Uint64* outSize) {
	Uint64 size;
	SDL_GetStorageFileSize(storage, path.data(), &size);

	auto data = new char[size];
	SDL_ReadStorageFile(storage, path.data(), data, size);

	if (outSize != nullptr) *outSize = size;
	return data;
}

yyjson_doc* LoadJson(SDL_Storage* storage, const std::string& path) {
	Uint64 size;
	char* jsonStr = LoadText(storage, path, &size);
	yyjson_doc* doc = yyjson_read(jsonStr, size, 0);
	delete[] jsonStr;
	return doc;
}
}  // namespace ResourceManager
