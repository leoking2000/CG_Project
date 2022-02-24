#pragma once
#include "utilities/GL_Math.h"
#include "FrameBuffer.h"
#include <GLFW/glfw3.h>

namespace GL
{
	class Window
	{
	public:
		Window(u32 window_width, u32 window_height, const char* window_title, bool resizable);

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		~Window();
	public:
		// The key is a GLFW key macro
		bool KeyIsPress(u32 key);

		bool MouseButtonIsPress(u32 key);

		bool ShouldWindowClose();

		// makes ShouldWindowClose return true
		void CloseWindow();

		void ResizeWindow(u32 width, u32 height);

		glm::vec2 WindowSize();

		glm::vec2 MousePos();

		void SetMouseVisibility(bool visible);

	public:
		FrameBuffer* fb;

		// glfw_window is public in order to initialize imgui and swap buffers in the engine
		GLFWwindow* glfw_window = nullptr;
	};
}