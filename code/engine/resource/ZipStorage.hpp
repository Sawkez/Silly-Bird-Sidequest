#pragma once

#include <SDL3/SDL.h>

#include <cstring>
#include <iostream>
#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ZipStream.hpp"
#include "zip.h"

namespace ZipStorage {
inline SDL_StorageInterface interface;

SDL_Storage* Open(char* data, Sint64 size) {
	auto* zip = new ZipStream(data, size);

	if (zip->stream == nullptr) {
		return nullptr;
	}

	return SDL_OpenStorage(&interface, zip);
}

SDL_Storage* Open(const std::string& path) {
	SDL_IOStream* stream = SDL_IOFromFile(path.c_str(), "r");
	Sint64 size = SDL_GetIOSize(stream);
	char* data = new char[size];
	SDL_ReadIO(stream, data, size);
	return Open(data, size);
}

SDL_Storage* Open(SDL_Storage* parentStorage, const std::string& path) {
	auto* zip = new ZipStream(parentStorage, path);

	if (zip->stream == nullptr) {
		return nullptr;
	}

	return SDL_OpenStorage(&interface, zip);
}

bool Close(void* userData) {
	delete (ZipStream*)userData;
	return true;
}

bool Ready(void* userData) { return true; }

bool Enumerate(void* userData, const char* directory, SDL_EnumerateDirectoryCallback callback, void* callbackUserdata) {
	auto* zip = ((ZipStream*)userData)->zip;
	int count = zip_entries_total(zip);

	if (count < 0) {
		dc::err << "ERROR getting zip entry count: " << zip_strerror(count) << dc::endl;
		return false;
	}

	for (int i = 0; i < count; i++) {
		zip_entry_openbyindex(zip, i);	// FIXME would this even give directories???

		const char* entryPath = zip_entry_name(zip);

		int dirLength = strlen(directory);
		const char* fileNameStart = entryPath + dirLength + 1;	// skip the /

		if (strncmp(entryPath, directory, dirLength) != 0) {  // entry is not in directory
			zip_entry_close(zip);
			continue;
		}
		if (strchr(fileNameStart, '/') != nullptr) {  // entry is in a subdirectory
			zip_entry_close(zip);
			continue;
		}

		int entryLength = strlen(entryPath);

		if (entryLength == dirLength) {	 // entry is the directory itself
			zip_entry_close(zip);
			continue;
		}

		char fileName[256];
		strcpy(fileName, fileNameStart);

		SDL_EnumerationResult result = callback(callbackUserdata, directory, fileName);
		switch (result) {
			case SDL_ENUM_FAILURE:
				zip_entry_close(zip);
				return false;

			case SDL_ENUM_SUCCESS:
				zip_entry_close(zip);
				return true;
		}

		zip_entry_close(zip);
	}

	return true;
}

bool Info(void* userData, const char* path, SDL_PathInfo* info) {
	auto* zip = ((ZipStream*)userData)->zip;
	int error = zip_entry_open(zip, path);
	if (error < 0) {
		dc::err << "ERROR getting info for zip entry " << path << ": " << zip_strerror(error) << dc::endl;
		return false;
	}

	info->type = zip_entry_isdir(zip) ? SDL_PATHTYPE_DIRECTORY : SDL_PATHTYPE_FILE;
	info->size = zip_entry_uncomp_size(zip);

	// i don't think we need, or can get, the accessed / created / modified times

	zip_entry_close(zip);

	return true;
}

bool ReadFile(void* userData, const char* path, void* destination, Uint64 length) {
	auto* zip = ((ZipStream*)userData)->zip;
	int error = zip_entry_open(zip, path);
	if (error < 0) {
		dc::err << "ERROR opening zip entry " << path << ": " << zip_strerror(error) << dc::endl;
		return false;
	}

	zip_entry_noallocread(zip, destination, length);

	zip_entry_close(zip);

	return true;
}

void Init() {
	SDL_INIT_INTERFACE(&interface);
	interface.close = Close;
	interface.ready = Ready;
	interface.enumerate = Enumerate;
	interface.info = Info;
	interface.read_file = ReadFile;
}

}  // namespace ZipStorage