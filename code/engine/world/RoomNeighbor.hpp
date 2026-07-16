#pragma once

#include <SDL3/SDL.h>

#include "engine/resource/BinaryReader.hpp"
#include "yyjson.h"

struct RoomNeighbor : public SDL_FRect {
	Uint32 index;

	RoomNeighbor() : SDL_FRect{0.0, 0.0, 0.0, 0.0}, index(0) {}

	RoomNeighbor(BinaryReader& binary) {
		Sint64 X, Y;
		binary.Read(8, &X);
		binary.Read(8, &Y);

		Uint16 W, H;
		binary.Read(2, &W);
		binary.Read(2, &H);

		x = float(X);
		y = float(Y);
		w = float(W);
		h = float(H);

		binary.Read(4, &index);
	}

	RoomNeighbor(yyjson_val* json)
		: SDL_FRect{
			  float(yyjson_get_num(yyjson_obj_get(json, "x"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "y"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "width"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "height"))),
		  },
		  index(yyjson_get_int(yyjson_obj_get(json, "index"))) {}
};