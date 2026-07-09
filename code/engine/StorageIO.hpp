#pragma once
#include <SDL3/SDL.h>

#include <string>

class StorageIO {
   private:
	char* _data;
	Uint64 _size;

   public:
	SDL_IOStream* stream;

	StorageIO(char* data, Uint64 size) : _data(data), _size(size), stream(SDL_IOFromConstMem(_data, size)) {}

	StorageIO(SDL_Storage* storage, const std::string& path) {
		SDL_GetStorageFileSize(storage, path.c_str(), &_size);
		_data = new char[_size];
		SDL_ReadStorageFile(storage, path.c_str(), _data, _size);
		stream = SDL_IOFromConstMem(_data, _size);
	}

	~StorageIO() {
		SDL_CloseIO(stream);
		delete[] _data;
	}
};