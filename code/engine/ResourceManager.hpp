#pragma once

#include <SDL3/SDL.h>

#include <string>

#include "yyjson.h"

namespace ResourceManager {

std::string LoadText(const std::string& path, int* outSize) {
	SDL_IOStream* file{SDL_IOFromFile(path.c_str(), "r")};

	int size = SDL_GetIOSize(file);
	std::string text;
	text.resize(size);

	SDL_ReadIO(file, text.data(), size);
	SDL_CloseIO(file);

	if (outSize != nullptr) *outSize = size;
	return text;
}

yyjson_doc* LoadJson(const std::string& path) {
	int size;
	std::string data = LoadText(path, &size);
	return yyjson_read(data.data(), size, 0);
}
}  // namespace ResourceManager