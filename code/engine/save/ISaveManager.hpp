#pragma once

class ISaveManager {
   public:
	virtual void ShowMenu() = 0;
	virtual void Draw() = 0;
	virtual bool OverrideDrawing() const = 0;
};