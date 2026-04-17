#pragma once

#include <string>

#include "engine/ui/DirectoryListMenu.hpp"
#include "lvgl/lvgl.h"

class ModSelectMenu : public DirectoryListMenu {
	static void SelectedCallback(lv_event_t* event) {
		UIManager::ClearStack();
		WorldManager::LoadLevel(*(std::string*)lv_event_get_user_data(event));
	}

	lv_event_cb_t GetSelectedCallback() const override { return SelectedCallback; }
	std::string GetDirectoryToList() const override { return "mods"; }
};