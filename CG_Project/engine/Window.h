#pragma once
#include "VE_Math.h"
#include <GLFW/glfw3.h>

namespace VE
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
		GLFWwindow* glfw_window = nullptr;
	};
}