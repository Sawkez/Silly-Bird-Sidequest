#pragma once

#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "zip.h"

struct ZipStream {
	zip_t* zip;
	char* stream;
	Uint64 size;

	ZipStream(const std::string& path) {
		stream = ResourceManager::LoadFile(ResourceManager::gameData, path, &size);
		int error;
		zip = zip_stream_openwitherror(stream, size, 0, 'r', &error);
		if (error < 0) dc::err << "ERROR: " << zip_strerror(error) << dc::endl;
	}

	~ZipStream() {
		delete[] stream;
		zip_stream_close(zip);
	}
};