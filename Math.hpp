#pragma once

namespace Math {

    int Wrap(int value, int min, int max) {
        int len = max - min + 1;

        if (value < max) {
            value += len * ((min - value) / len + 1);
        }

        return min + (value - min) % len;
    }

}