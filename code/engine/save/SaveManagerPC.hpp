#pragma once

#include <filesystem>
#include <fstream>

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/save/SaveManagerBase.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/save/ListSaveMenu.hpp"

class SaveManagerPC : public SaveManagerBase, public ISaveManagerPC {
   private:
	ListSaveMenu _saveMenu;

   public:
	void Init() override {
		std::filesystem::create_directories(std::filesystem::path(GetUserDir() + "/manual"));
		ListSaveMenu::_manager = this;
		_saveMenu.Init();
	}

	void ShowSaveMenu() override { UIManager::Push(&_saveMenu); }

	void ShowLoadMenu() override {}
	void Autosave() override {}
	void Autoload() override {}

	void SaveToDirectory(const std::string& path) override {
		std::filesystem::create_directories(std::filesystem::path(path));

		std::ofstream file(path + "/DATA.BIN", std::ios::binary | std::ios::out);
		file.write((char*)&saveData, sizeof(saveData));
	}

	void LoadFromDirectory(const std::string& path) override {}

	std::string GetUserDir() const override {
#ifdef _WIN32

		const char* appdata = std::getenv("APPDATA");
		return std::string(appdata) + "\\" + "Silly Bird Sidequest"

#elif defined(__linux__)

		const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
		if (xdgDataHome != NULL) {
			return std::string(xdgDataHome) + "/" + "Silly Bird Sidequest";

		} else {
			const char* home = std::getenv("HOME");
			return std::string(home) + "/.local/share/Silly Bird Sidequest";
		}

#endif
	}

	void Draw() override {}
	bool OverrideDrawing() const override { return false; }
};