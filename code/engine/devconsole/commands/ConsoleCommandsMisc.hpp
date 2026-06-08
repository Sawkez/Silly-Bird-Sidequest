#pragma once

#include <string>

#include "engine/devconsole/DevConsole.hpp"

namespace ConsoleCommandsMisc {

void Help(const std::string& args) { dc::msg << "I haven't added any commands yet but thanks for asking" << dc::endl; }

void Typo(const std::string& args) { dc::err << "Command not found: " << args << dc::endl; }

}  // namespace ConsoleCommandsMisc