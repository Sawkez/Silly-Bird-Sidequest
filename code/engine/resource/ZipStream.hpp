#pragma once

#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "zip.h"

struct ZipStream {
   private:
	char* ReadIO(SDL_IOStream* stream) {
		size = SDL_GetIOSize(stream);
		char* data = new char[size];
		SDL_ReadIO(stream, data, size);
		return data;
	}

   public:
	char* stream;
	Uint64 size;
	zip_t* zip;

	ZipStream(char* zipStream, Uint64 size) : size(size) {
		stream = zipStream;
		int error;
		zip = zip_stream_openwitherror(stream, size, 0, 'r', &error);
		if (error < 0) dc::err << "ERROR: " << zip_strerror(error) << dc::endl;
	}

	ZipStream(SDL_Storage* storage, const std::string& path) : stream(ResourceManager::LoadFile(storage, path, &size)) {
		int error;
		zip = zip_stream_openwitherror(stream, size, 0, 'r', &error);
		if (error < 0) dc::err << "ERROR: " << zip_strerror(error) << dc::endl;
	}

	~ZipStream() {
		delete[] stream;
		zip_stream_close(zip);
	}
};