#pragma once

#include <string>

class IDevConsoleVariable {
   public:
	virtual void Set(const std::string& value) = 0;
	virtual std::string GetString() const = 0;
	virtual bool IsCheat() const = 0;
	virtual bool IsUnsafe() const = 0;
};