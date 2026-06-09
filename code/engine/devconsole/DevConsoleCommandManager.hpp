#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleCommand.hpp"

class DevConsoleCommandManager {
   private:
	static inline const int COMMAND_COUNT = 2;
	static inline DevConsoleCommand _commands[COMMAND_COUNT];

#ifdef PLATFORM_HAS_STRING_COMMANDS
	static inline std::unordered_map<std::string, const DevConsoleCommand&> _commandsByName;
#endif

   public:
	static void RegisterCommand(const std::string& name, void (*function)(const std::vector<std::string>&), unsigned char flags, int index,
								const std::string& description) {
		if (index >= COMMAND_COUNT) std::cerr << "ERROR REGISTERING COMMAND: INDEX " << index << "OUT OF RANGE" << std::endl;
		_commands[index] = DevConsoleCommand(function, flags, description);

#ifdef PLATFORM_HAS_STRING_COMMANDS
		_commandsByName.insert({name, _commands[index]});
#endif
	}

	static void RunCommand(int command, const std::vector<std::string>& args, bool fromUser) { _commands[command].Run(args, fromUser); }

	static void RunCommand(const std::string& command, bool fromUser) {
		// Stripping leading spaces
		int commandNameStart = 0;
		while (commandNameStart < command.length() && command[commandNameStart] == ' ') {
			commandNameStart++;
		}

		// Separating command name (up until first space)
		int commandNameEnd = commandNameStart + 1;
		while (commandNameEnd < command.length() && command[commandNameEnd] != ' ') {
			commandNameEnd++;
		}
		std::string commandName = command.substr(commandNameStart, commandNameEnd - commandNameStart);

		std::vector<std::string> args;

		int argStart = commandNameEnd + 1;

		// Separating arguments
		while (true) {
			while (argStart < command.length() && command[argStart] == ' ') {
				argStart++;
			}

			int argEnd = argStart;
			while (argEnd < command.length() && command[argEnd] != ' ') {
				argEnd++;
			}

			if (argEnd == argStart) break;
			std::string abc = command.substr(argStart, argEnd - argStart);
			args.push_back(command.substr(argStart, argEnd - argStart));

			argStart = argEnd + 1;
		}

		auto commandFunction = _commandsByName.find(commandName);

		if (commandFunction == _commandsByName.end()) {
			dc::err << "Command not found: " << commandName << dc::endl;
			return;
		}

		commandFunction->second.Run(args, fromUser);
	}

	static void ListCommands() {
		for (const auto& pair : _commandsByName) {
			dc::msg << pair.first << ": " << pair.second.GetDescription() << dc::endl;
		}
	}
};