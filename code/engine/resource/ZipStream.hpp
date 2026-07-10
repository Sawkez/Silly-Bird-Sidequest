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
	zip_t* zip;
	char* stream;
	Uint64 size;

	ZipStream(char* zipStream) {
		stream = zipStream;
		int error;
		zip = zip_stream_openwitherror(zipStream, size, 0, 'r', &error);
		if (error < 0) dc::err << "ERROR: " << zip_strerror(error) << dc::endl;
	}

	ZipStream(SDL_Storage* storage, const std::string& path)
		: ZipStream(ResourceManager::LoadFile(storage, path, &size)) {}

	ZipStream(const std::string& path) : ZipStream(ReadIO(SDL_IOFromFile(path.c_str(), "r"))) {}

	~ZipStream() {
		delete[] stream;
		zip_stream_close(zip);
	}
};