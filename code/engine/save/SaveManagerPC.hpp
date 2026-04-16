#pragma once

#include "engine/save/SaveManagerBase.hpp"

class SaveManagerPC : public SaveManagerBase {
	void Init() override {}
	void ShowSaveMenu() override {}
	void ShowLoadMenu() override {}
	void Draw() override {}
	bool OverrideDrawing() const override { return false; }
};