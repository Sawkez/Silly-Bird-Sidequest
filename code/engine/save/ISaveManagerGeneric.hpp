#pragma once

#include <string>

#include "SDL3/SDL.h"

class ISaveManagerGeneric {
   public:
	virtual SDL_Storage* OpenUserDir() const = 0;
	virtual void LoadFromDirectory(const std::string& path) = 0;
	virtual void SaveToDirectory(const std::string& path) = 0;
	virtual void NewSave() = 0;
	virtual void Autoload() = 0;
};