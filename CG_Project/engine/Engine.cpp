#include "Engine.h"

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace GL
{
	Engine::Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable)
		:
		win(window_width, window_height, window_title, resizable)
	{
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(win.glfw_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	Engine::~Engine()
	{
		ModelManager::Clear();
		TextureManager::Clear();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		LogInfo("Destroyed Engine");
	}

	float Engine::ElapsedTime()
	{
		return elapsed.count() / 1000000.0f;
	}

	void Engine::NewFrame()
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		currentTime = std::chrono::high_resolution_clock::now();
		elapsed = currentTime - previousTime;
		continous_time += (elapsed.count() / 1000000.0f) / 1000.0f;
	}

	void Engine::EndFrame()
	{
		renderer.RenderToScreen(win, continous_time);

		previousTime = currentTime;
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(win.glfw_window);
	}

}
