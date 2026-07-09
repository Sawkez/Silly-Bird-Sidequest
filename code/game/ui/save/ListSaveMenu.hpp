#pragma once

#include "engine/save/ISaveManagerGeneric.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ListSaveMenu : public DirectoryListMenu {
   private:
	friend class SaveManagerGeneric;
	static inline ISaveManagerGeneric* _manager = nullptr;

	static inline const int SPECIAL_INDEX_NEW = -1;

	static void SelectedCallback(lv_event_t* event) {
		int index = *(int*)lv_event_get_user_data(event);

		if (index == SPECIAL_INDEX_NEW) {
			_manager->NewSave();
		}

		else {
			_manager->SaveToDirectory(_paths[index]);
		}

		UIManager::Pop();
	}

	static SDL_EnumerationResult AddPath(void* userdata, const char* dirname, const char* fname) {
		auto* paths = (std::vector<std::string>*)userdata;
		paths->emplace_back(fname);
		return SDL_ENUM_CONTINUE;
	}

   public:
	void Activate() override {
		DirectoryListMenu::Activate();
		AddButton("New save", SPECIAL_INDEX_NEW);
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }

	void UpdatePaths() override {
		SDL_Storage* userDir = _manager->OpenUserDir();
		SDL_EnumerateStorageDirectory(userDir, "manual", AddPath, &_paths);
		SDL_CloseStorage(userDir);
	}
};