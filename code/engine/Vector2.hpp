#pragma once

#include <cmath>
#include <iostream>

#include "yyjson.h"

class Vector2 : public SDL_FPoint {
	static constexpr float ZERO_PRECISION = 0.001;

  public:
	Vector2(float X, float Y) : SDL_FPoint{X, Y} {}

	Vector2(const SDL_Point& point) : SDL_FPoint{float(point.x), float(point.y)} {}

	Vector2() : Vector2(0.0, 0.0) {}

	Vector2(yyjson_val* vectorJson)
		: SDL_FPoint{float(yyjson_get_num(yyjson_obj_get(vectorJson, "x"))),
					 float(yyjson_get_num(yyjson_obj_get(vectorJson, "y")))} {}

	float LengthSquared() const { return x * x + y * y; }

	float Length() const { return sqrtf(LengthSquared()); }

	Vector2 Normalized() const {
		float len = Length();
		if (len == 0.0)
			return *this;
		return Vector2{x / len, y / len};
	}

	Vector2 Rotated(float angle) const {
		return {cosf(angle) * x - sinf(angle) * y, sinf(angle) * x - cosf(angle) * y};
	}

	float DistanceSquared(const Vector2& other) const { return (*this - other).LengthSquared(); }

	float Distance(const Vector2& other) const { return (*this - other).Length(); }

	Vector2 DirectionTo(const Vector2& other) const { return (other - *this).Normalized(); }

	float Angle() const {
		float angle = atan2(y, x);
		return angle;
	}

	bool IsZeroApprox() const { return abs(x) + abs(y) < ZERO_PRECISION; }

	void MoveToward(const Vector2& target, float moveDistance) {
		if (DistanceSquared(target) < moveDistance * moveDistance)
			*this = target;
		*this += DirectionTo(target) * moveDistance;
	}

	Vector2 operator+(const Vector2& b) const { return Vector2{x + b.x, y + b.y}; }

	Vector2 operator-(const Vector2& b) const { return Vector2{x - b.x, y - b.y}; }

	Vector2 operator-() const { return Vector2{-x, -y}; }

	Vector2 operator*(float b) const { return Vector2{x * b, y * b}; }

	Vector2 operator/(float b) const { return Vector2{x / b, y / b}; }

	Vector2 operator*(const Vector2& b) { return Vector2{x * b.x, y * b.y}; }

	Vector2& operator+=(const Vector2& b) {
		x += b.x;
		y += b.y;
		return *this;
	}

	Vector2& operator*=(float b) {
		x *= b;
		y *= b;
		return *this;
	}

	Vector2& operator*=(const Vector2& b) {
		x *= b.x;
		y *= b.y;
		return *this;
	}

	bool operator==(const Vector2& b) const { return x == b.x && y == b.y; }
	bool operator!=(const Vector2& b) const { return !(*this == b); }

	float& operator[](int index) { return index == 0 ? x : y; }

	static const Vector2 ZERO;
	static const Vector2 UP;
	static const Vector2 DOWN;
	static const Vector2 LEFT;
	static const Vector2 RIGHT;
};

inline const Vector2 Vector2::ZERO = Vector2(0.0, 0.0);
inline const Vector2 Vector2::UP = Vector2(0.0, -1.0);
inline const Vector2 Vector2::DOWN = Vector2(0.0, 1.0);
inline const Vector2 Vector2::LEFT = Vector2(-1.0, 0.0);
inline const Vector2 Vector2::RIGHT = Vector2(1.0, 0.0);

Vector2 operator*(float a, const Vector2& b) { return Vector2{a * b.x, a * b.y}; }

std::ostream& operator<<(std::ostream& out, const Vector2& vec) {
	out << "( " << vec.x << ", " << vec.y << " )";
	return out;
}