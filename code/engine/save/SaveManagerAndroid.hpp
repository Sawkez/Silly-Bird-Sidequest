#pragma once

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>

#include "engine/save/ISaveManagerPC.hpp"
#include "engine/save/SaveManagerBase.hpp"
#include "engine/ui/UIManager.hpp"
#include "game/ui/save/ListLoadMenu.hpp"
#include "game/ui/save/ListSaveMenu.hpp"

class SaveManagerAndroid : public SaveManagerBase {
	void Init() override {}
	void ShowSaveMenu() override {}
	void ShowLoadMenu() override {}
	void Autosave() override {}
	void Draw() override {}
	bool OverrideDrawing() const override { return false; }
};