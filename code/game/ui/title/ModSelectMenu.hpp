#pragma once

#include <string>
#include <vector>

#include "engine/PlatformDefines.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ModSelectMenu : public DirectoryListMenu {
   private:
	static inline const int SPECIAL_INDEX_OPEN_FOLDER = -1;
	static inline const int SPECIAL_INDEX_SELECT_FOLDER = -2;

	static void ModPathSelectedCallback(void* userData, const char* const* files, int filter) {
		if (files == nullptr) return;

		ResourceManager::OpenModFolder(files);
		UIManager::Pop();
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

			default:
				ModManager::LoadLevelMod(ResourceManager::mods, _paths[index]);
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
#ifdef PLATFORM_HAS_FILE_PICKER
	void Activate() override {
		DirectoryListMenu::Activate();
		if (ResourceManager::mods != nullptr) {
			AddButton("Open mod folder", SPECIAL_INDEX_OPEN_FOLDER);
		}
		AddButton("Select mod folder", SPECIAL_INDEX_SELECT_FOLDER);
	}
#endif

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }

	void UpdatePaths() override {
		if (ResourceManager::mods == nullptr) return;
		SDL_EnumerateStorageDirectory(ResourceManager::mods, nullptr, AddPath, &_paths);
	}
};