#pragma once

#include <ctime>
#include <filesystem>
#include <iomanip>

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ListLoadMenu : public DirectoryListMenu {
	friend class SaveManagerPC;
	static inline ISaveManagerPC* _manager = nullptr;

	static void SelectedCallback(lv_event_t* event) {
		auto* saveName = (std::string*)lv_event_get_user_data(event);
		if (*saveName == "") {
			_manager->LoadFromDirectory(_manager->GetAutosaveDir());
		}

		else {
			_manager->LoadFromDirectory(*saveName);
		}
	}

   public:
	void Init() override {
		DirectoryListMenu::Init();

		if (!std::filesystem::exists(_manager->GetAutosaveDir())) return;

		_buttons.emplace_back(_panel, "", "Autosave", GetSelectedCallback());

		lv_obj_move_to_index(_buttons.back().GetButton(), 0);
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }
	std::string GetDirectoryToList() const override { return _manager->GetManualSaveDir(); }
};