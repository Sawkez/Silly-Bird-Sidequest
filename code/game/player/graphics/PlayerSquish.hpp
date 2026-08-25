#pragma once

#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"

#define CONVAR_CATEGORY PLAYER_SQUISH

namespace PlayerSquish {
// clang-format off
    CONVAR(float,   duck,       DUCK,       0.5f,   DC_FLAG_NONE);
    CONVAR(float,   standUp,    STAND_UP,   1.5f,   DC_FLAG_NONE);
    CONVAR(float,   twerkDown,  TWERK_DOWN, 0.9f,   DC_FLAG_NONE);
    CONVAR(float,   twerkUp,    TWERK_UP,   1.1f,   DC_FLAG_NONE);
    CONVAR(float,   slide,      SLIDE,      0.5f,   DC_FLAG_NONE);
// clang-format on
}  // namespace PlayerSquish

#undef CONVAR_CATEGORY