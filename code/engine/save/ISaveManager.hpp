#pragma once

class ISaveManager {
   public:
	virtual void ShowMenu() = 0;
	virtual void Process() = 0;
};