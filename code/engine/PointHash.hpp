#pragma once

#include <SDL3/SDL.h>

#include <functional>

template <>
struct std::hash<SDL_Point> {
	std::size_t operator()(const SDL_Point& p) const noexcept {
		std::size_t h1 = std::hash<int>{}(p.x);
		std::size_t h2 = std::hash<int>{}(p.y);
		return h1 ^ (h2 << 1);
	}
};