#include "OpenGL.h"
#include "Window.h"
#include "utilities/log.h"

void window_size_callback(GLFWwindow* window, int width, int height)
{
	GL::Window* win = reinterpret_cast<GL::Window*>(glfwGetWindowUserPointer(window));

	win->ResizeWindow(width, height);
}

namespace GL
{
	Window::Window(u32 window_width, u32 window_height, const char* window_title, bool resizable)
	{
		// Set GLFW error callback
		glfwSetErrorCallback(GL::LogGLFW_Error);

		// Initialize GLFW
		if (!glfwInit())
		{
			LogError("GLFW could not Initialize.");
			return;
		}

		// Hits
		// OpenGL version 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

		// Create Window
		glfw_window = glfwCreateWindow(window_width, window_height, window_title, NULL, NULL);
		if (!glfw_window)
		{
			LogError("Window creation failed.");
			glfwTerminate();
			return;
		}

		glfwSetWindowSizeCallback(glfw_window, window_size_callback);
		glfwMakeContextCurrent(glfw_window);

		glfwSetWindowUserPointer(glfw_window, this);

		// Initialize OpenGL
		if (!InitOpenGL())
		{
			LogError("OpenGL could not Initialize.");
			return;
		}

		// print OpenGL version
		LogInfo("Using OpenGL");
		LogInfo((const char*)glGetString(GL_VERSION));

		// enable vsync
		glfwSwapInterval(1);

		glCall(glViewport(0, 0, window_width, window_height));

		fb = new FrameBuffer(window_width, window_height);
	}

	Window::~Window()
	{
		delete fb; // this needs to be deleted before we destory the window

		glfwDestroyWindow(glfw_window);
		glfwTerminate();

		LogInfo("Destroyed Window");
	}

	bool Window::KeyIsPress(u32 key)
	{
		return glfwGetKey(glfw_window, (int)key) == GLFW_PRESS;
	}

	bool Window::MouseButtonIsPress(u32 key)
	{
		return glfwGetMouseButton(glfw_window, key) == GLFW_PRESS;
	}

	bool Window::ShouldWindowClose()
	{
		return glfwWindowShouldClose(glfw_window);
	}

	void Window::CloseWindow()
	{
		glfwSetWindowShouldClose(glfw_window, true);
	}

	void Window::ResizeWindow(u32 width, u32 height)
	{
		glfwSetWindowSize(glfw_window, (int)width, (int)height);

		glCall(glViewport(0, 0, width, height));

		fb->Resize(width, height);
	}

	glm::vec2 Window::WindowSize()
	{
		int width, height;
		glfwGetWindowSize(glfw_window, &width, &height);

		return glm::vec2((float)width, (float)height);
	}

	glm::vec2 Window::MousePos()
	{
		double xpos, ypos;
		glfwGetCursorPos(glfw_window, &xpos, &ypos);

		return glm::vec2((float)xpos, (float)ypos);
	}

	void Window::SetMouseVisibility(bool visible)
	{
		glfwSetInputMode(glfw_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
}


