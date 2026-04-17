#pragma once

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ListSaveMenu : public DirectoryListMenu {
	friend class SaveManagerPC;
	static inline ISaveManagerPC* _manager = nullptr;

	static void SelectedCallback(lv_event_t* event) {
		auto* saveName = (std::string*)lv_event_get_user_data(event);
		if (*saveName == "") {
			_manager->NewSave();
		}

		else {
			_manager->SaveToDirectory(*saveName);
		}

		UIManager::Pop();
	}

   public:
	void Init() override {
		DirectoryListMenu::Init();

		_buttons.emplace_back(_panel, "", "New save", GetSelectedCallback());
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }
	std::string GetDirectoryToList() const override { return _manager->GetManualSaveDir(); }
};