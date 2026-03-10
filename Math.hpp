#pragma once

#include <SDL.h>

#include "Vector2.hpp"

namespace Math {

int Wrap(int value, int min, int max) {
	int len = max - min + 1;

	if (value < max) {
		value += len * ((min - value) / len + 1);
	}

	return min + (value - min) % len;
}

float CopySignOrZero(float magnitude, float sign) {
	if (sign == 0.0)
		return 0.0;
	return copysignf(magnitude, sign);
}

float SignOrZero(float value) {
	if (value == 0.0)
		return 0.0;
	else if (signbit(value))
		return -1.0;
	else
		return 1.0;
}

float Radians(float degrees) { return degrees * M_PI / 180.0; }

float MoveTowards(float a, float b, float moveSpeed) {
	if (fabsf(a - b) < moveSpeed)
		return b;
	float move = CopySignOrZero(moveSpeed, b - a);
	return a + move;
}

float Lerp(float a, float b, float ratio) { return a + ratio * (b - a); }

SDL_FRect ScaleRect(const SDL_FRect& rect, const Vector2& scaleOrigin, const Vector2& scale) {
	return {rect.x - scaleOrigin.x * scale.x + scaleOrigin.x, rect.y - scaleOrigin.y * scale.y + scaleOrigin.y,
			rect.w * scale.x, rect.h * scale.y};
}

SDL_FRect ScaleRect(const SDL_FRect& rect, const Vector2& scaleOrigin, float scale) {
	return ScaleRect(rect, scaleOrigin, Vector2{scale, scale});
}

} // namespace Math