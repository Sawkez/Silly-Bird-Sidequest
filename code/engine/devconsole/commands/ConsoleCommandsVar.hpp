#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/variable/DevConsoleVariableManager.hpp"

namespace ConsoleCommandsVar {

void set(const std::vector<std::string>& args, bool fromUser) {
	if (args.size() < 2) {
		dc::err << "Usage: set <VARIABLE_NAME> <value>" << dc::endl;
		return;
	}

	DevConsoleVariableManager::Set(args[0], args[1], fromUser);
}
}  // namespace ConsoleCommandsVar