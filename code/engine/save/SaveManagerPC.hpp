#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/save/SaveManagerBase.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/save/ListLoadMenu.hpp"
#include "game/ui/save/ListSaveMenu.hpp"

#ifdef _WIN32
#define OS_DIR_SEPARATOR "\\"

#elifdef __linux__
#define OS_DIR_SEPARATOR "/"

#endif

class SaveManagerPC : public SaveManagerBase, public ISaveManagerPC {
   private:
	ListSaveMenu _saveMenu;
	ListLoadMenu _loadMenu;

   public:
	void Init() override {
		std::filesystem::create_directories(std::filesystem::path(GetUserDir() + "/manual"));
		ListSaveMenu::_manager = this;
		ListLoadMenu::_manager = this;
		_saveMenu.Init();
		_loadMenu.Init();
	}

	void ShowSaveMenu() override { UIManager::Push(&_saveMenu); }
	void ShowLoadMenu() override { UIManager::Push(&_loadMenu); }
	void Autosave() override { SaveToDirectory(GetUserDir() + "/auto"); }

	void SaveToDirectory(const std::string& path) override {
		std::filesystem::create_directories(std::filesystem::path(path));

		std::ofstream file(path + "/DATA.BIN", std::ios::binary | std::ios::out);
		file.write((char*)&saveData, sizeof(SaveData));
	}

	void LoadFromDirectory(const std::string& path) override {
		std::ifstream file(path + "/DATA.BIN", std::ios::binary | std::ios::in);
		file.read((char*)&saveData, sizeof(SaveData));
		_loadedCallback();
	}

#ifdef _WIN32
	std::string GetUserDir() const override {
		const char* appdata = std::getenv("APPDATA");
		return std::string(appdata) + "\\Silly Bird Sidequest";
	}

	std::string GetManualSaveDir() const override { return GetUserDir() + "\\manual"; }

	std::string GetAutosaveDir() const override { return GetUserDir() + "\\auto"; }

#elifdef __linux__
	std::string GetUserDir() const override {
		const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
		if (xdgDataHome != NULL) {
			return std::string(xdgDataHome) + "/Silly Bird Sidequest";

		} else {
			const char* home = std::getenv("HOME");
			return std::string(home) + "/.local/share/Silly Bird Sidequest";
		}
	}

	std::string GetManualSaveDir() const override { return GetUserDir() + "/manual"; }

	std::string GetAutosaveDir() const override { return GetUserDir() + "/auto"; }
#endif

	virtual void NewSave() override {
		time_t time = std::time(nullptr);
		tm* localTime = std::localtime(&time);
		std::ostringstream oss;
		oss << std::put_time(localTime, "%d%m%Y_%H_%M_%S");

		SaveToDirectory(GetManualSaveDir() + OS_DIR_SEPARATOR + oss.str());
	}

	void Draw() override {}
	bool OverrideDrawing() const override { return false; }
};