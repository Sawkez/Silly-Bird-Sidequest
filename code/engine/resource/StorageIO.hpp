#pragma once
#include <SDL3/SDL.h>

#include <string>

#include "engine/devconsole/DevConsole.hpp"

class StorageIO {
   private:
	char* _data;
	Uint64 _size;

   public:
	SDL_IOStream* stream;

	StorageIO(char* data, Uint64 size) : _data(data), _size(size), stream(SDL_IOFromConstMem(_data, size)) {}

	StorageIO(SDL_Storage* storage, const std::string& path) {
		while (!SDL_StorageReady(storage)) {
			SDL_Delay(1);
		}

		if (!SDL_GetStorageFileSize(storage, path.c_str(), &_size)) {
			dc::err << "ERROR: " << SDL_GetError() << dc::endl;
		}

		_data = new char[_size];
		if (!SDL_ReadStorageFile(storage, path.c_str(), _data, _size)) {
			dc::err << "ERROR: " << SDL_GetError() << dc::endl;
		}

		stream = SDL_IOFromConstMem(_data, _size);
		if (stream == nullptr) {
			dc::err << "ERROR loading file " << path << ": " << SDL_GetError() << dc::endl;
		}
	}

	~StorageIO() {
		SDL_CloseIO(stream);
		delete[] _data;
	}
};