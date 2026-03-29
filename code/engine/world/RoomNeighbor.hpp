#pragma once

#include <SDL.h>

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