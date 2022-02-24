#pragma once
#include "OpenGL.h"
#include "utilities/log.h"

namespace GL
{
	bool InitOpenGL()
	{
		static bool first_call = true;

		if (!first_call)
		{
			LogError("InitOpenGL was called more than one time!!!");
			return false;
		}

		first_call = false;

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
}

