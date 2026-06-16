#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"

namespace ConsoleCommandsMisc {

void help(const std::vector<std::string>& args) { DevConsoleCommandManager::ListCommands(); }

void clear(const std::vector<std::string>& args) { DevConsole::Clear(); }

}  // namespace ConsoleCommandsMisc