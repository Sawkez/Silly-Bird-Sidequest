#pragma once

namespace Math {

    int Wrap(int value, int min, int max) {
        int len = max - min + 1;

        if (value < max) {
            value += len * ((min - value) / len + 1);
        }

        return min + (value - min) % len;
    }

    float CopySignOrZero(float magnitude, float sign) {
        if (sign == 0.0) return 0.0;
        return copysignf(magnitude, sign);
    }

    float SignOrZero(float value) {
        if (value == 0.0) return 0.0;
        else if (signbit(value)) return -1.0;
        else return 1.0;
    }

}