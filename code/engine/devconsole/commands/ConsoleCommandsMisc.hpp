#pragma once

#include <string>
#include <vector>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleCommandManager.hpp"

namespace ConsoleCommandsMisc {

void help(const std::vector<std::string>& args, bool fromUser) { DevConsoleCommandManager::ListCommands(); }

void clear(const std::vector<std::string>& args, bool fromUser) { DevConsole::Clear(); }

}  // namespace ConsoleCommandsMisc