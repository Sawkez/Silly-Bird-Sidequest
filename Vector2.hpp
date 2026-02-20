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

        Vector2 operator+(const Vector2& b) const {
            return Vector2{
                x + b.x,
                y + b.y
            };
        }

        Vector2 operator*(float b) const {
            return Vector2{
                x * b,
                y * b
            };
        }

        Vector2& operator+=(const Vector2& b) {
            x = x + b.x;
            y = y + b.y;
            return *this;
        }

        bool operator==(const Vector2& b) const {
            return x == b.x && y == b.y;
        }
};

inline const Vector2 Vector2::ZERO = Vector2(0.0, 0.0);

Vector2 operator*(float a, const Vector2& b) {
    return Vector2 {a * b.x, a * b.y};
}