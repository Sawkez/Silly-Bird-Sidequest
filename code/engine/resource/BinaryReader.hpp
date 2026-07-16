#pragma once

#include <SDL3/SDL.h>

#include <cstring>
#include <string>

class BinaryReader {
   private:
	char* _data;
	Uint64 _size;
	char* _sectionData = nullptr;
	char* _currentPosition = nullptr;
	char* _sectionName = nullptr;
	Uint32 _sectionLength = 0;

   public:
	BinaryReader(SDL_Storage* storage, const std::string& path) {
		SDL_GetStorageFileSize(storage, path.c_str(), &_size);
		_data = new char[_size];
		SDL_ReadStorageFile(storage, path.c_str(), _data, _size);

		_sectionName = _data;
		memcpy(&_sectionLength, _data + 4, 4);
		_sectionData = _data + 4 + 4;
		_currentPosition = _sectionData;
	}

	BinaryReader(const BinaryReader&) = delete;
	BinaryReader& operator=(const BinaryReader&) = delete;

	BinaryReader& operator=(BinaryReader&& other) noexcept = default;

	void GetNextSection() {
		_sectionName = _sectionData + _sectionLength;
		memcpy(&_sectionLength, _sectionName + 4, 4);
		_sectionData = _sectionName + 4 + 4;
		_currentPosition = _sectionData;
	}

	void FindNextSection(const char tag[4]) {
		do GetNextSection();
		while (strncmp(_sectionName, tag, 4) != 0);
	}

	void FindSection(const char tag[4]) {
		while (strncmp(_sectionName, tag, 4) != 0) {
			GetNextSection();
		}
	}

	const char* GetCurrentPosition() const { return _currentPosition; }
	void AdvanceCurrentPosition(int num) { _currentPosition += num; }

	void Read(int bytes, void* outData) {
		memcpy(outData, _currentPosition, bytes);
		_currentPosition += bytes;
	}

	~BinaryReader() { delete[] _data; }
};