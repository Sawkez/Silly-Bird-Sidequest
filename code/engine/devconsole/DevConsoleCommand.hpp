#pragma once

#include <string>
#include <vector>

class DevConsoleCommand {
   private:
	unsigned char _flags = 0;
	void (*_function)(const std::vector<std::string>& args) = nullptr;

   public:
	static inline const unsigned char FLAG_CHEAT = 1 << 0;
	static inline const unsigned char FLAG_UNSAFE = 1 << 1;

	DevConsoleCommand() {}
	DevConsoleCommand(void (*function)(const std::vector<std::string>& args), unsigned char flags) : _flags(flags), _function(function) {}

	void Run(const std::vector<std::string>& args, bool fromUser) const { _function(args); }
};