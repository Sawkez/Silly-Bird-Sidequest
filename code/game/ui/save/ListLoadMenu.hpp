#pragma once

#include <ctime>
#include <filesystem>
#include <iomanip>

#include "engine/save/ISaveManagerGeneric.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ListLoadMenu : public DirectoryListMenu {
   private:
	friend class SaveManagerGeneric;
	static inline ISaveManagerGeneric* _manager = nullptr;

	static inline const int SPECIAL_INDEX_AUTO = -1;

	static void SelectedCallback(lv_event_t* event) {
		auto index = *(int*)lv_event_get_user_data(event);

		if (index == SPECIAL_INDEX_AUTO) {
			_manager->Autoload();
		}

		else {
			_manager->LoadFromDirectory(_paths[index]);
		};

		UIManager::ClearStack();
	}

	static SDL_EnumerationResult AddPath(void* userdata, const char* dirname, const char* fname) {
		auto* paths = (std::vector<std::string>*)userdata;
		paths->emplace_back(fname);
		return SDL_ENUM_CONTINUE;
	}

   public:
	void Activate() override {
		DirectoryListMenu::Activate();
		AddButton("Autosave", SPECIAL_INDEX_AUTO);
		_buttons.back()->MoveToIndex(0);
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }

	void UpdatePaths() override {
		SDL_Storage* userDir = _manager->OpenUserDir();
		SDL_EnumerateStorageDirectory(userDir, "manual", AddPath, &_paths);
		SDL_CloseStorage(userDir);
	}
};