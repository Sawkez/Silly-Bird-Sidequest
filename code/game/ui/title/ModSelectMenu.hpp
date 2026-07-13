#pragma once

#include <string>
#include <vector>

#include "engine/PlatformDefines.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/ZipStorage.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ModSelectMenu : public DirectoryListMenu {
   private:
	static inline const int SPECIAL_INDEX_OPEN_FOLDER = -1;
	static inline const int SPECIAL_INDEX_SELECT_FOLDER = -2;
	static inline const int SPECIAL_INDEX_SELECT_FILE = -3;

	static void ModPathSelectedCallback(void* userData, const char* const* files, int filter) {
		if (files == nullptr) return;

		ResourceManager::OpenModFolder(files);
		UIManager::Pop();
	}

	static void ModSelectedCallback(char* data, Sint64 size) {
		SDL_Storage* zip = ZipStorage::Open(data, size);

		ModManager::LoadLevelModFromStorage(zip, "");
		UIManager::Push(UIManager::MENU_LEVELS);
	}

	static void SelectedCallback(lv_event_t* event) {
		auto index = *(int*)lv_event_get_user_data(event);

		switch (index) {
			case SPECIAL_INDEX_OPEN_FOLDER:
				SDL_OpenURL(("file://" + ResourceManager::modFolderPath).c_str());
				break;

			case SPECIAL_INDEX_SELECT_FOLDER:
				SDL_ShowOpenFolderDialog(ModPathSelectedCallback, nullptr, GameState::GetMainWindow(), nullptr, false);
				break;

			case SPECIAL_INDEX_SELECT_FILE:
				ResourceManager::OpenFilePicker(ModSelectedCallback, GameState::GetMainWindow());
				break;

			default:
				ModManager::LoadLevelModFromFolder(ResourceManager::mods, _paths[index]);
				UIManager::Push(UIManager::MENU_LEVELS);
				break;
		}
	}

	static SDL_EnumerationResult AddPath(void* userData, const char* dirName, const char* fileName) {
		auto* paths = (std::vector<std::string>*)userData;
		paths->emplace_back(fileName);
		return SDL_ENUM_CONTINUE;
	}

   public:
	void Activate() override {
		DirectoryListMenu::Activate();

#ifdef PLATFORM_HAS_FILE_EXPLORER
		if (ResourceManager::mods != nullptr) {
			AddButton("Open mod folder", SPECIAL_INDEX_OPEN_FOLDER);
		}
#endif
#ifdef PLATFORM_HAS_FOLDER_PICKER
		AddButton("Select mod folder", SPECIAL_INDEX_SELECT_FOLDER);
#endif
#ifdef PLATFORM_HAS_FILE_PICKER
		AddButton("Select mod file", SPECIAL_INDEX_SELECT_FILE);
#endif
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }

	void UpdatePaths() override {
		if (ResourceManager::mods == nullptr) return;
		if (!SDL_EnumerateStorageDirectory(ResourceManager::mods, nullptr, AddPath, &_paths)) {
			dc::err << "ERROR enumerating path " << ResourceManager::modFolderPath << ": " << SDL_GetError()
					<< dc::endl;
		}
	}
};