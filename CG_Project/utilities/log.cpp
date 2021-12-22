#include "log.h"
#include <stdio.h>

// logs a string into the console with a new line at the end.
void Log(const std::string& msg)
{
		printf("%s\n", msg.c_str());
}

// logs a string into the console with a "[INFO]" at the beginning and a new line at the end.
void LogInfo(const std::string& msg)
{
	printf("[INFO] %s\n", msg.c_str());
}

// logs a string into the console with a "[ERROR]" at the beginning and a new line at the end.
void LogError(const std::string& msg)
{
	printf("[ERROR] %s\n", msg.c_str());
}

// logs a c style string into the console with a new line at the end.
void Log(const char* msg)
{
	printf("%s\n", msg);
}

void LogGLFW_Error(int code, const char* msg)
{
	printf("[GLFW ERROR] code %i \n%s\n", code, msg);
}

