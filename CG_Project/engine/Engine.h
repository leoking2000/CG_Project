#pragma once
#include <chrono>
#include "Renderer.h"
#include "Camera.h"

#include "../imgui/imgui.h"

namespace GL
{
	class Engine
	{
	public:
		Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable);

		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		virtual ~Engine();

		virtual void Start() = 0;
	protected:	
		float ElapsedTime(); // in microseconds
		void NewFrame();
		void EndFrame();
	protected:
		Window win;
		Renderer renderer;
		Camera cam;

		std::vector<GameObject> objets;
	private:
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds elapsed;
		f32 continous_time = 0;
	};
}