#pragma once

#include <SDL3/SDL.h>

#include <cstring>
#include <memory>
#include <string>

class BinaryReader {
   private:
	std::unique_ptr<char[]> _data;
	Uint64 _size;
	char* _sectionData = nullptr;
	char* _currentPosition = nullptr;
	char* _sectionName = nullptr;
	Uint32 _sectionLength = 0;

   public:
	BinaryReader(SDL_Storage* storage, const std::string& path) {
		SDL_GetStorageFileSize(storage, path.c_str(), &_size);
		_data = std::make_unique<char[]>(_size);
		SDL_ReadStorageFile(storage, path.c_str(), _data.get(), _size);

		_sectionName = _data.get();
		memcpy(&_sectionLength, _data.get() + 4, 4);
		_sectionData = _data.get() + 4 + 4;
		_currentPosition = _sectionData;
	}

	BinaryReader(const BinaryReader&) = delete;
	BinaryReader& operator=(const BinaryReader&) = delete;

	BinaryReader(BinaryReader&&) noexcept = default;
	BinaryReader& operator=(BinaryReader&&) noexcept = default;

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

	void EnsureSection(const char tag[4]) {
		while (strncmp(_sectionName, tag, 4) != 0) {
			GetNextSection();
		}
	}

	const char* GetCurrentPosition() const { return _currentPosition; }
	void AdvanceCurrentPosition(int num) { _currentPosition += num; }

	void GoToChunkAtPosition(const char* position) {
		_currentPosition = (char*)position;
		_sectionData = (char*)position;
		memcpy(&_sectionLength, _sectionData - 4, 4);
		_sectionName = _sectionData - 4 - 4;
	}

	void Read(int bytes, void* outData) {
		memcpy(outData, _currentPosition, bytes);
		_currentPosition += bytes;
	}

	void Skip(int bytes) { _currentPosition += bytes; }
};