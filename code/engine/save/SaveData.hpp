#pragma once

#include <SDL.h>
#include <string.h>

#include <iostream>
#include <string>

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

std::ostream& operator<<(std::ostream& out, const SaveData& save) {
	out << "Mod path: " << save.modPath << std::endl
		<< "Room: " << save.room << std::endl
		<< "Checkpoint: " << save.checkpoint << std::endl
		<< "Upgrades: " << int(save.upgrades) << std::endl;

	return out;
}