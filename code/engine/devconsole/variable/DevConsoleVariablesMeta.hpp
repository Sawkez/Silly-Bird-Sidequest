#pragma once

#include "engine/devconsole/variable/DevConsoleVariable.hpp"

namespace DevConsoleVariablesMeta {
CONVAR(bool, enableCheats, META_ENABLE_CHEATS, false, DC_FLAG_UNSAFE, "Enable cheat commands");
}