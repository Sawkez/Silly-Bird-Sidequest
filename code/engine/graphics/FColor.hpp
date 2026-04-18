#pragma once

#include <SDL3/SDL.h>

#include "engine/Math.hpp"

struct FColor : SDL_FColor {
	FColor(float red, float green, float blue) : SDL_FColor{red, green, blue, 1.0f} {}

	FColor(Uint8 red, Uint8 green, Uint8 blue) : SDL_FColor{float(red) / 255.0f, float(green) / 255.0f, float(blue) / 255.0f, 1.0f} {}

	FColor(int red, int green, int blue) : FColor(Uint8(red), Uint8(green), Uint8(blue)) {}

	FColor(SDL_Color color) : FColor(color.r, color.g, color.b) {}

	SDL_Color GetIntColor() const { return {Uint8(r * 255), Uint8(g * 255), Uint8(b * 255), 255}; }

	float& operator[](int index) {
		switch (index) {
			case 0:
				return r;
			case 1:
				return g;
			case 2:
				return b;
		}
	}

	void MoveToward(const FColor& other, float moveSpeed) {
		r = Math::MoveTowards(r, other.r, moveSpeed);
		g = Math::MoveTowards(g, other.g, moveSpeed);
		b = Math::MoveTowards(b, other.b, moveSpeed);
	}
};