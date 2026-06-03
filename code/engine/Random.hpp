#pragma once

#include <SDL3/SDL.h>
#include <time.h>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/devconsole/DevConsole.hpp"

namespace Random {

void Init() {
	std::srand(time(NULL));
	dc::msg << "First random number: " << std::rand() << dc::endl;
}

int Int(int min, int max) { return min + std::rand() % (max - min + 1); }

float Float(float min, float max) { return Math::Lerp(min, max, float(std::rand()) / float(RAND_MAX)); }

float Angle() { return Float(-M_PI, M_PI); }

Vector2 Direction() {
	float angle = Angle();
	return Vector2(cosf(angle), sinf(angle));
}

}  // namespace Random