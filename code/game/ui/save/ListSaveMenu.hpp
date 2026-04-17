#pragma once

#include <ctime>
#include <iomanip>

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ListSaveMenu : public DirectoryListMenu {
	friend class SaveManagerPC;
	static inline ISaveManagerPC* _manager = nullptr;

	static void SelectedCallback(lv_event_t* event) {
		auto* saveName = (std::string*)lv_event_get_user_data(event);
		if (*saveName == "") {
			time_t time = std::time(nullptr);
			tm* localTime = std::localtime(&time);
			std::ostringstream oss;
			oss << std::put_time(localTime, "%d%m%Y_%H_%M_%S");

			_manager->SaveToDirectory(_manager->GetUserDir() + "/manual/" + oss.str());
		}

		else {
			_manager->SaveToDirectory(*saveName);
		}
	}

   public:
	void Init() override {
		DirectoryListMenu::Init();

		_buttons.emplace_back(_panel, "", "New save", GetSelectedCallback());
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }
	std::string GetDirectoryToList() const override { return _manager->GetUserDir() + "/manual"; }
};