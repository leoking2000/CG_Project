#include "Engine.h"

namespace GL
{
	Engine::Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable)
		:
		win(window_width, window_height, window_title, resizable)
	{
	}

	Engine::~Engine()
	{
		ModelManager::Clear();
		TextureManager::Clear();

		LogInfo("Destroyed Engine");
	}

	float Engine::ElapsedTime()
	{
		return elapsed.count() / 1000000.0f;
	}

	void Engine::NewFrame()
	{
		glfwPollEvents();

		currentTime = std::chrono::high_resolution_clock::now();
		elapsed = currentTime - previousTime;
		continous_time += (elapsed.count() / 1000000.0f) / 1000.0f;
	}

	void Engine::EndFrame()
	{
		renderer.RenderToScreen(win, continous_time);

		previousTime = currentTime;
		glfwSwapBuffers(win.glfw_window);
	}

}
