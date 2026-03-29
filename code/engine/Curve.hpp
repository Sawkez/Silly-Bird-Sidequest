#pragma once

#include <cmath>
#include <fstream>
#include <iostream>

#include "engine/Math.hpp"

using namespace std;

class Curve {
	unsigned short _pointCount = 0;
	float* _points;

   public:
	Curve(string path) : Curve(ifstream(path)) {}

	Curve(ifstream file) {
		file.read(reinterpret_cast<char*>(&_pointCount), 2);
		_points = new float[_pointCount];

		for (unsigned short i = 0; i < _pointCount; i++) {
			file.read(reinterpret_cast<char*>(_points + i), 4);
		}
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