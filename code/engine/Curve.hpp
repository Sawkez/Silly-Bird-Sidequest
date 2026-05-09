#pragma once

#include <SDL3/SDL.h>

#include <cmath>
#include <fstream>
#include <iostream>

#include "engine/Math.hpp"

using namespace std;

class Curve {
	unsigned short _pointCount = 0;
	float* _points;

   public:
	Curve(string path) : Curve(SDL_IOFromFile(path.c_str(), "rb")) {}

	Curve(SDL_IOStream* file) {
		SDL_ReadIO(file, &_pointCount, 2);
		_points = new float[_pointCount];

		SDL_ReadIO(file, _points, _pointCount * 4);

		SDL_CloseIO(file);
	}

	float Sample(float x) const {
		if (x < 0.0) return _points[0];
		if (x > 1.0) return _points[_pointCount - 1];

		unsigned short sampleIdx = x * _pointCount;
		return _points[sampleIdx];
	}

	float LerpSample(float x) const {
		if (x < 0.0) return _points[0];
		if (x > 1.0) return _points[_pointCount - 1];

		x *= (_pointCount - 1);	 // -1 so we don't go oob when sampling upper point
		unsigned short sampleLow = static_cast<unsigned short>(x);

		float ratio = x - sampleLow;

		return Math::Lerp(_points[sampleLow], _points[sampleLow + 1], ratio);
	}

	~Curve() { delete[] _points; }
};