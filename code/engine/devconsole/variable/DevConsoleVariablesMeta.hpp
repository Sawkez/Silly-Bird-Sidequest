#pragma once

#include "engine/devconsole/variable/DevConsoleVariable.hpp"

#define CONVAR_CATEGORY META

namespace DevConsoleVariablesMeta {
CONVAR(bool, enableCheats, ENABLE_CHEATS, false, DC_FLAG_UNSAFE, "Enable cheat commands");
}

#undef CONVAR_CATEGORY