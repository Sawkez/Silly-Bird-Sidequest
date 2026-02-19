#pragma once

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

        static const Vector2 ZERO;

        Vector2 operator+(Vector2 b) {
            return Vector2{
                x + b.x,
                y + b.y
            };
        }

        Vector2 operator*(float b) {
            return Vector2{
                x * b,
                y * b
            };
        }
};

inline const Vector2 Vector2::ZERO = Vector2(0.0, 0.0);