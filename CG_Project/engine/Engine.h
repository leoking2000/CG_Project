#pragma once
#include <chrono>
#include "graphics/OpenGL.h"
#include "graphics/Window.h"
#include "../imgui/imgui.h"

#include "utilities/log.h"
#include "Camera.h"
#include "Model.h"
#include "graphics/Shader.h"
#include "graphics/FrameBuffer.h"
#include "graphics/Texture.h"

#include "GameObject.h"

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

		void reloadShaders();

		Window win;
		std::vector<GameObject> objets;
		Camera cam;

		// sky
		std::string sky_map;

	private:
		// light
		const glm::mat4 light_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
		const glm::mat4 light_view = glm::lookAt(glm::vec3(0.0f, 450.0f, -150.0f), glm::vec3(0.0f, 0.0f, -130.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::vec3 light_dir = -glm::normalize(glm::vec3(0.0f, 500.0f, -100.0f));
		const glm::mat4 lightSpaceMatrix;

		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds elapsed;
		f32 continous_time = 0;
	private:
		Shader basic{ "Shader/rendering.glsl" };

		// post process
		Shader post_process{ "Shader/post_process.glsl" };
		VertexArray vao_post_process;

		// Shadows
		Shader shadow_shader{ "Shader/shadow_map.glsl" };
		static constexpr u32 SHADOW_SIZE = 4096;
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

		//skybox
		Shader sky_shader{ "Shader/sky.glsl" };
		Mesh sky_sphere{ GenarateSphere() };
	private:
		void RenderShadowMap();
		void RenderGeometry();
		void PostProccess();

	};
}