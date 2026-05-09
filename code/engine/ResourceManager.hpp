#pragma once

#include <SDL3/SDL.h>

#include <string>

#include "yyjson.h"

namespace ResourceManager {

std::string LoadText(const std::string& path, int* outSize) {
	size_t size;
	void* data = SDL_LoadFile(path.c_str(), &size);

	std::string text(static_cast<char*>(data), size);
	SDL_free(data);

	if (outSize != nullptr) *outSize = static_cast<int>(size);
	return text;
}

yyjson_doc* LoadJson(const std::string& path) {
	int size;
	std::string data = LoadText(path, &size);
	return yyjson_read(data.data(), size, 0);
}
}  // namespace ResourceManager
