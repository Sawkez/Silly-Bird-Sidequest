#pragma once

enum DevConsoleFlags : unsigned char {
	DC_FLAG_NONE = 0,
	DC_FLAG_CHEAT = 1 << 0,
	DC_FLAG_UNSAFE = 1 << 1,
};