#pragma once

#include <functional>

#include "engine/save/SaveData.hpp"

class SaveManagerBase {
   protected:
	std::function<void()> _loadedCallback;

   public:
	SaveData saveData = SaveData();

	virtual void Init() = 0;

	virtual void ShowSaveMenu() = 0;
	virtual void ShowLoadMenu() = 0;
	virtual void Autosave() = 0;

	virtual void Draw() = 0;
	virtual bool OverrideDrawing() const = 0;

	void SetLoadedCallback(std::function<void()> callback) { _loadedCallback = std::move(callback); }
};