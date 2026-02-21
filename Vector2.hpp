#pragma once

#include <iostream>

class Vector2 {
    public:
        float x;
        float y;

        Vector2(float X, float Y) :
        x(X), y(Y)
        { }

        Vector2() :
        Vector2(0.0, 0.0)
        { }

        Vector2 operator+(const Vector2& b) const {
            return Vector2{
                x + b.x,
                y + b.y
            };
        }

        Vector2 operator-(const Vector2& b) const {
            return Vector2{
                x - b.x,
                y - b.y
            };
        }

        Vector2 operator*(float b) const {
            return Vector2{
                x * b,
                y * b
            };
        }

        Vector2 operator*(const Vector2& b) {
            return Vector2{
                x * b.x,
                y * b.y
            };
        }

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

        bool operator==(const Vector2& b) const {
            return x == b.x && y == b.y;
        }

        float& operator[](int index) {
            return index == 0? x : y;
        }

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

Vector2 operator*(float a, const Vector2& b) {
    return Vector2 {a * b.x, a * b.y};
}

std::ostream& operator<<(std::ostream& out, const Vector2& vec) {
    out << "( " << vec.x << ", " << vec.y << " )";
    return out;
}