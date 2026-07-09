#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "engine/save/ISaveManagerGeneric.hpp"
#include "engine/save/SaveManagerBase.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/save/ListLoadMenu.hpp"
#include "game/ui/save/ListSaveMenu.hpp"

class SaveManagerGeneric : public SaveManagerBase, public ISaveManagerGeneric {
   private:
	ListSaveMenu _saveMenu;
	ListLoadMenu _loadMenu;

	static inline const char ORGANIZATION[] = "noentertainment";
	static inline const char APPLICATION[] = "sbsidequest";

   public:
	SDL_Storage* OpenUserDir() const override {
		SDL_Storage* userDir = SDL_OpenUserStorage(ORGANIZATION, APPLICATION, 0);
		while (!SDL_StorageReady(userDir));
		return userDir;
	}

	void Init() override {
		ListSaveMenu::_manager = this;
		ListLoadMenu::_manager = this;
		_saveMenu.Init();
		_loadMenu.Init();
	}

	void ShowSaveMenu() override { UIManager::Push(&_saveMenu); }
	void ShowLoadMenu() override { UIManager::Push(&_loadMenu); }
	void Autosave() override { SaveToDirectory("auto"); }
	void Autoload() override { LoadFromDirectory("auto"); }

	void SaveToDirectory(const std::string& path) override {
		SDL_Storage* userDir = OpenUserDir();
		SDL_CreateStorageDirectory(userDir, path.c_str());
		SDL_WriteStorageFile(userDir, (path + "/DATA.BIN").c_str(), &saveData, sizeof(SaveData));
		SDL_CloseStorage(userDir);
	}

	void LoadFromDirectory(const std::string& path) override {
		SDL_Storage* userDir = OpenUserDir();
		SDL_ReadStorageFile(userDir, (path + "/DATA.BIN").c_str(), &saveData, sizeof(SaveData));
		SDL_CloseStorage(userDir);

		_loadedCallback();
	}

	virtual void NewSave() override {
		time_t time = std::time(nullptr);
		tm* localTime = std::localtime(&time);
		std::ostringstream oss;
		oss << std::put_time(localTime, "%d%m%Y_%H_%M_%S");

		SaveToDirectory("manual/" + oss.str());
	}

	void Draw() override {}
	bool OverrideDrawing() const override { return false; }
};