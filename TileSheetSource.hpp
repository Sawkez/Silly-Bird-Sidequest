#pragma once

#include <string>

struct TileSheetSource {
	bool custom;
	std::string name;

	TileSheetSource() : custom(false), name("") {}

	std::string GetPath(const std::string& modPath) const {
		if (custom)
			return modPath + "/tiles/fg/" + name;
		else
			return "content/sidequest/tiles/fg/" + name;
	}
};