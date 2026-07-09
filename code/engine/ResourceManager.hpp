#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/StorageIO.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "yyjson.h"

namespace ResourceManager {

char* LoadText(SDL_Storage* storage, const std::string& path, Uint64* outSize) {
	Uint64 size;
	SDL_GetStorageFileSize(storage, path.c_str(), &size);

	auto data = new char[size];
	SDL_ReadStorageFile(storage, path.c_str(), data, size);

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

SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_Storage* storage, const std::string& path) {
	dc::msg << " ??? " << path << dc::endl;
	return IMG_LoadTexture_IO(renderer, StorageIO(storage, path).stream, false);
}
}  // namespace ResourceManager
