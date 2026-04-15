#pragma once

class ISaveManager {
   public:
	virtual void Init() = 0;
	virtual void ShowSaveMenu() = 0;
	virtual void Draw() = 0;
	virtual bool OverrideDrawing() const = 0;
};