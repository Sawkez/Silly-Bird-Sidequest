#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/StorageIO.hpp"
#include "yyjson.h"

#if SDL_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

class ResourceManager {
   public:
	static inline const char ORGANIZATION[] = "noentertainment";
	static inline const char APPLICATION[] = "sbsidequest";

	static inline SDL_Storage* gameData;
	static inline SDL_Storage* mods = nullptr;
	static inline std::string modFolderPath = "";

	static void Init() {
		gameData = SDL_OpenTitleStorage(nullptr, 0);
		while (gameData != nullptr && !SDL_StorageReady(gameData)) {
			SDL_Delay(1);
		}

#if SDL_PLATFORM_PSP
		modFolderPath = "mods";
		mods = SDL_OpenTitleStorage(modFolderPath.c_str(), 0);
#else
		modFolderPath = SDL_GetPrefPath(ORGANIZATION, APPLICATION) + std::string("/mods");
		mods = SDL_OpenTitleStorage(modFolderPath.c_str(), 0);
#endif
	}

	static char* LoadFile(SDL_Storage* storage, const std::string& path, Uint64* outSize) {
		while (!SDL_StorageReady(storage)) {
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

	static yyjson_doc* LoadJson(SDL_Storage* storage, const std::string& path) {
		Uint64 size;
		char* jsonStr = LoadFile(storage, path, &size);
		yyjson_doc* doc = yyjson_read(jsonStr, size, 0);
		delete[] jsonStr;
		return doc;
	}

	static SDL_Surface* LoadSurface(SDL_Storage* storage, const std::string& path) {
		SDL_Surface* surface = IMG_Load_IO(StorageIO(storage, path).stream, false);
		if (surface == nullptr) dc::err << "ERROR loading surface " << path << ": " << SDL_GetError() << dc::endl;
		return surface;
	}

	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_Storage* storage, const std::string& path) {
		SDL_Texture* texture = IMG_LoadTexture_IO(renderer, StorageIO(storage, path).stream, false);
		if (texture == nullptr) dc::err << "ERROR loading texture " << path << ": " << SDL_GetError() << dc::endl;
		return texture;
	}

	static void OpenModFolder(const char* const* path) {
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

#if SDL_PLATFORM_EMSCRIPTEN
	static inline void (*filePickedCallback)(char* data, Sint64 size) = nullptr;

	static void OpenFilePicker(void (*callback)(char* data, Sint64 size), SDL_Window* window) {
		filePickedCallback = callback;
		EM_ASM({ openFilePicker(); });
	}

#else
   private:
	static void FilePickedCallback(void* userData, const char* const* files, int filter) {
		auto* callback = (void (*)(char*, Sint64))userData;
		SDL_IOStream* stream = SDL_IOFromFile(files[0], "r");
		Sint64 size = SDL_GetIOSize(stream);
		char* data = new char[size];
		SDL_ReadIO(stream, data, size);
		callback(data, size);
	}

   public:
	static void OpenFilePicker(void (*callback)(char* data, Sint64 size), SDL_Window* window) {
		SDL_ShowOpenFileDialog(FilePickedCallback, (void*)callback, window, nullptr, 0, nullptr, false);
	}
#endif
};

#if SDL_PLATFORM_EMSCRIPTEN
extern "C" void on_file_picked(const char* path) {
	SDL_IOStream* stream = SDL_IOFromFile(path, "r");
	Sint64 size = SDL_GetIOSize(stream);
	char* data = new char[size];
	SDL_ReadIO(stream, data, size);

	ResourceManager::filePickedCallback(data, size);
}
#endif