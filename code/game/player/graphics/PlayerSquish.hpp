#pragma once

#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"

#define CONVAR_CATEGORY PLAYER_SQUISH

namespace PlayerSquish {

// clang-format off
CONVAR(float,	baseVelocityX,	BASE_VELOCITY_X,	200.0f,			DC_FLAG_NONE);
CONVAR(float,	baseVelocityY,	BASE_VELOCITY_Y,	250.0f,			DC_FLAG_NONE);
CONVAR(float,	acceleration,	ACCELERATION,		180.0f,			DC_FLAG_NONE);
CONVAR(float,	maxVelocity,	MAX_VELOCITY,		150.0f,			DC_FLAG_NONE);
CONVAR(float,	minVelocity,	MIN_VELOCITY,		0.25f,			DC_FLAG_NONE);
CONVAR(float,	dampening,		DAMPENING,			0.000058228f,	DC_FLAG_NONE);
CONVAR(float,	xMin,			X_MIN,				0.25,			DC_FLAG_NONE);
CONVAR(float,	xMax,			X_MAX,				1.5,			DC_FLAG_NONE);
CONVAR(float,	xReset,			X_RESET,			0.075,			DC_FLAG_NONE);
CONVAR(float,	yMax,			Y_MAX,				1.25,			DC_FLAG_NONE);
CONVAR(float,	duck,			DUCK,				0.5f,			DC_FLAG_NONE);
CONVAR(float,	standUp,		STAND_UP,			1.5f,			DC_FLAG_NONE);
CONVAR(float,	twerkDown,		TWERK_DOWN,			0.9f,			DC_FLAG_NONE);
CONVAR(float,	twerkUp,		TWERK_UP,			1.1f,			DC_FLAG_NONE);
CONVAR(float,	slide,			SLIDE,				0.5f,			DC_FLAG_NONE);
// clang-format on

}  // namespace PlayerSquish

#undef CONVAR_CATEGORY