#pragma once

#include <string>

class ISaveManagerPC {
   public:
	virtual void LoadFromDirectory(const std::string& path) = 0;
	virtual void SaveToDirectory(const std::string& path) = 0;
	virtual std::string GetUserDir() const = 0;
	virtual std::string GetManualSaveDir() const = 0;
	virtual std::string GetAutosaveDir() const = 0;
	virtual void NewSave() = 0;
};