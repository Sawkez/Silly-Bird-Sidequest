#pragma once

#include <SDL3/SDL.h>

#include <cstring>
#include <iostream>
#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "zip.h"

namespace ZipStorage {

inline SDL_StorageInterface interface;

SDL_Storage* Open(const std::string& path) {
	zip_t* zip = zip_open(path.c_str(), 0, 'r');
	if (zip == nullptr) {
		// dc::err << "ERROR opening zip " << path << ": " << zip_strerror(error) << dc::endl;
		return nullptr;
	}

	int count = zip_entries_total(zip);

	dc::msg << "Contents of " << path << ":" << dc::endl;

	for (int i = 0; i < count; i++) {
		zip_entry_openbyindex(zip, i);
		dc::msg << zip_entry_name(zip) << dc::endl;
		zip_entry_close(zip);
	}

	return SDL_OpenStorage(&interface, zip);
}

bool Close(void* userData) {
	zip_close((zip_t*)userData);
	return true;
}

bool Ready(void* userData) { return true; }

bool Enumerate(void* userData, const char* directory, SDL_EnumerateDirectoryCallback callback, void* callbackUserdata) {
	auto* zip = (zip_t*)userData;
	int count = zip_entries_total(zip);

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
	auto* zip = (zip_t*)userData;
	if (zip_entry_open(zip, path) < 0) return false;

	info->type = zip_entry_isdir(zip) ? SDL_PATHTYPE_DIRECTORY : SDL_PATHTYPE_FILE;
	info->size = zip_entry_uncomp_size(zip);

	// i don't think we need, or can get, the accessed / created / modified times

	zip_entry_close(zip);

	return true;
}

bool ReadFile(void* userData, const char* path, void* destination, Uint64 length) {
	auto* zip = (zip_t*)userData;
	if (zip_entry_open(zip, path) < 0) return false;

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