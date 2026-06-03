#pragma once

#include <SDL3/SDL.h>
#include <string.h>

#include <iostream>
#include <string>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleOutputStream.hpp"

// #include "yyjson.h"

struct SaveData {
	char modPath[300];
	Uint8 upgrades;
	int room;
	int checkpoint;

	SaveData() : SaveData("", 0, 0, 0) {}

	SaveData(const char* ModPath, Uint8 Upgrades, int Room, int Checkpoint) {
		strcpy(modPath, ModPath);
		upgrades = Upgrades;
		room = Room;
		checkpoint = Checkpoint;
	}

	void SetPath(const std::string& path) { strcpy(modPath, path.c_str()); }
};

DevConsoleOutputStream& operator<<(DevConsoleOutputStream& out, const SaveData& save) {
	out << "Mod path: " << save.modPath << dc::endl
		<< "Room: " << save.room << dc::endl
		<< "Checkpoint: " << save.checkpoint << dc::endl
		<< "Upgrades: " << int(save.upgrades) << dc::endl;

	return out;
}