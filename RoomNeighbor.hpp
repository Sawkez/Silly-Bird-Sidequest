#pragma once

#include <SDL.h>

#include "Json.hpp"
#include "yyjson.h"

struct RoomNeighbor : public SDL_FRect {
	int index;

	RoomNeighbor() : SDL_FRect{0.0, 0.0, 0.0, 0.0}, index(0) {}

	RoomNeighbor(yyjson_val* json)
		: SDL_FRect{
			float(yyjson_get_num(yyjson_obj_get(json, "x"))),
			float(yyjson_get_num(yyjson_obj_get(json, "y"))),
			float(yyjson_get_num(yyjson_obj_get(json, "width"))),
			float(yyjson_get_num(yyjson_obj_get(json, "height"))),
		},
		index(yyjson_get_int(yyjson_obj_get(json, "index"))) {}
};

void from_json(const nlohmann::json& json, RoomNeighbor& neighbor) {
	neighbor.x = json.at("x");
	neighbor.y = json.at("y");
	neighbor.w = json.at("width");
	neighbor.h = json.at("height");
	neighbor.index = json.at("index");
}