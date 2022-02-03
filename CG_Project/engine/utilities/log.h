#pragma once
#include <string>

namespace GL
{
	// logs a string into the console with a new line at the end.
	void Log(const std::string& msg);

	// logs a string into the console with a "[INFO]" at the beginning and a new line at the end.
	void LogInfo(const std::string& msg);

	// logs a string into the console with a "[ERROR]" at the beginning and a new line at the end.
	void LogError(const std::string& msg);

	// logs a c style string into the console with a new line at the end.
	void Log(const char* msg);

	void LogGLFW_Error(int code, const char* msg);
}

