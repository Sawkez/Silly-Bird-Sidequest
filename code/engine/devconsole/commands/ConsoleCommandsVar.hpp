#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/variable/DevConsoleVariableManager.hpp"

namespace ConsoleCommandsVar {

void set(const std::vector<std::string>& args, bool fromUser) {
	DevConsoleVariableManager::Set(args[0], args[1], fromUser);
}
}  // namespace ConsoleCommandsVar