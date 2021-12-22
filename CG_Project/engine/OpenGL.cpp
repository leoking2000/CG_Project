#pragma once
#include "OpenGL.h"
#include <string>
#include "Log.h"

bool InitOpenGL()
{
	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		LogError("GLEW could not Initialize.");
		return false;
	}

	return true;
}

void glClearError()
{
	while (glGetError());
}

bool glCheckError()
{
	bool haserror = false;
	while (GLenum error = glGetError())
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			LogError(std::to_string(error) + " <Invalid Enum>");
			break;
		case GL_INVALID_VALUE:
			LogError(std::to_string(error) + " <Invalid Value>");
			break;
		case GL_INVALID_OPERATION:
			LogError(std::to_string(error) + " <Invalid Operation>");
			break;
		default:
			LogError(std::to_string(error));
			break;
		}

		haserror = true;
	}

	return haserror;
}