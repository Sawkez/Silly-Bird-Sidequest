#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/StorageIO.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "yyjson.h"

namespace ResourceManager {

SDL_Storage* gameData;

void Init() {
	gameData = SDL_OpenTitleStorage(nullptr, 0);
	while (gameData != nullptr && !SDL_StorageReady(gameData)) {
		SDL_Delay(1);
	}
}

char* LoadFile(SDL_Storage* storage, const std::string& path, Uint64* outSize) {
	while (!SDL_StorageReady) {
		SDL_Delay(1);
	}

	Uint64 size;
	if (!SDL_GetStorageFileSize(storage, path.c_str(), &size)) {
		dc::err << "ERROR: " << SDL_GetError() << dc::endl;
	};

	auto data = new char[size];
	if (!SDL_ReadStorageFile(storage, path.c_str(), data, size)) {
		dc::err << "ERROR: " << SDL_GetError() << dc::endl;
	};

	if (outSize != nullptr) *outSize = size;
	return data;
}

yyjson_doc* LoadJson(SDL_Storage* storage, const std::string& path) {
	Uint64 size;
	char* jsonStr = LoadFile(storage, path, &size);
	yyjson_doc* doc = yyjson_read(jsonStr, size, 0);
	delete[] jsonStr;
	return doc;
}

SDL_Surface* LoadSurface(SDL_Storage* storage, const std::string& path) {
	SDL_Surface* surface = IMG_Load_IO(StorageIO(storage, path).stream, false);
	if (surface == nullptr) dc::err << "ERROR loading surface " << path << ": " << SDL_GetError() << dc::endl;
	return surface;
}

SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_Storage* storage, const std::string& path) {
	SDL_Texture* texture = IMG_LoadTexture_IO(renderer, StorageIO(storage, path).stream, false);
	if (texture == nullptr) dc::err << "ERROR loading texture " << path << ": " << SDL_GetError() << dc::endl;
	return texture;
}
}  // namespace ResourceManager
