#pragma once

#include <string>
#include <unordered_map>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsoleCommand.hpp"

class DevConsoleCommandManager {
   private:
	static inline const int COMMAND_COUNT = 2;
	static inline DevConsoleCommand _commands[COMMAND_COUNT];

#ifdef PLATFORM_HAS_STRING_COMMANDS
	static inline std::unordered_map<std::string, const DevConsoleCommand&> _commandsByName;
#endif

   public:
	static void RegisterCommand(const std::string& name, void (*function)(const std::string&), unsigned char flags, int index) {
		if (index >= COMMAND_COUNT) std::cerr << "ERROR REGISTERING COMMAND: INDEX " << index << "OUT OF RANGE" << std::endl;
		_commands[index] = DevConsoleCommand(function, flags);

#ifdef PLATFORM_HAS_STRING_COMMANDS
		_commandsByName.insert({name, _commands[index]});
#endif
	}

	static void RunCommand(int command, const std::string& args, bool fromUser) { _commands[command].Run(args, fromUser); }

	static void RunCommand(const std::string& command, bool fromUser) {
		// TODO delete leading/trailing/duplicate spaces
		// TODO split command from arguments

		auto commandFunction = _commandsByName.find(command);

		if (commandFunction == _commandsByName.end()) {
			RunCommand(0, command, fromUser);  // run Typo command
		}

		else {
			commandFunction->second.Run(command, fromUser);
		}
	}
};