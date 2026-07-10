#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/StorageIO.hpp"
#include "yyjson.h"

namespace ResourceManager {

inline const char ORGANIZATION[] = "noentertainment";
inline const char APPLICATION[] = "sbsidequest";

SDL_Storage* gameData;
SDL_Storage* mods = nullptr;
std::string modFolderPath = "";

void Init() {
	gameData = SDL_OpenTitleStorage(nullptr, 0);
	while (gameData != nullptr && !SDL_StorageReady(gameData)) {
		SDL_Delay(1);
	}

#if SDL_PLATFORM_PSP
	modFolderPath = "mods";
	mods = SDL_OpenTitleStorage(modFolderPath.c_str(), 0);
#elif SDL_PLATFORM_WINDOWS || SDL_PLATFORM_LINUX
	modFolderPath = SDL_GetPrefPath(ORGANIZATION, APPLICATION) + std::string("/mods");
	mods = SDL_OpenTitleStorage(modFolderPath.c_str(), 0);
#endif
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

void OpenModFolder(const char* const* path) {
	if (mods != nullptr) SDL_CloseStorage(mods);

	modFolderPath = path[0];
	mods = SDL_OpenTitleStorage(modFolderPath.c_str(), 0);
	if (mods == nullptr) {
		dc::err << "ERROR opening mod directory " << path[0] << ": " << SDL_GetError() << dc::endl;
	}

	while (!SDL_StorageReady(mods)) {
		SDL_Delay(1);
	}
}

}  // namespace ResourceManager
