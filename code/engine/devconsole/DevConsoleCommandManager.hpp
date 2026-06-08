#pragma once

#include <string>
#include <unordered_map>

#include "engine/PlatformDefines.hpp"

using Command = void (*)(const std::string&);

class DevConsoleCommandManager {
   private:
	static inline const int COMMAND_COUNT = 2;

	static inline Command _commands[COMMAND_COUNT];

#ifdef PLATFORM_HAS_STRING_COMMANDS
	static inline std::unordered_map<std::string, Command> _commandsByName;
#endif

   public:
	static void RegisterCommand(const std::string& name, Command command, int index) {
		_commands[index] = command;

#ifdef PLATFORM_HAS_STRING_COMMANDS
		_commandsByName[name] = command;
#endif
	}

	static void RunCommand(int command, const std::string& args) { _commands[command](args); }

	static void RunCommand(const std::string& command) {
		// TODO delete leading/trailing/duplicate spaces
		// TODO split command from arguments

		auto commandFunction = _commandsByName.find(command);

		if (commandFunction == _commandsByName.end()) {
			RunCommand(0, command);	 // run Typo command
		}

		else {
			commandFunction->second(command);
		}
	}
};