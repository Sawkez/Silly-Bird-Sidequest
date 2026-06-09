#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"

namespace ConsoleCommandsMisc {

void Help(const std::vector<std::string>& args) { DevConsoleCommandManager::ListCommands(); }

}  // namespace ConsoleCommandsMisc