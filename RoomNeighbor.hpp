#pragma once

#include <SDL.h>

#include "Json.hpp"

struct RoomNeighbor : public SDL_FRect {
	int index;

	RoomNeighbor() : SDL_FRect{0.0, 0.0, 0.0, 0.0}, index(0) {}
};

void from_json(const nlohmann::json& json, RoomNeighbor& neighbor) {
	neighbor.x = json.at("x");
	neighbor.y = json.at("y");
	neighbor.w = json.at("width");
	neighbor.h = json.at("height");
	neighbor.index = json.at("index");
}