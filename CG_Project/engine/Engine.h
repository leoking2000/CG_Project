#pragma once
#include <chrono>
#include "graphics/OpenGL.h"
#include "graphics/Window.h"
#include "../imgui/imgui.h"

#include "log.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/Shader.h"
#include "graphics/FrameBuffer.h"

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
	public:
		virtual void Start() = 0;
		// in microseconds
		float ElapsedTime();
	protected:
		void NewFrame();
		void EndFrame();
	protected:
		Window win;
		std::vector<GameObject> objets;
		Camera cam;

		// light
		glm::mat4 light_proj;
		glm::mat4 light_view;
		glm::vec3 light_dir;

		// sky
		std::string sky_map;
	private:
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds elapsed;
		f32 continous_time = 0;
	private:
		glm::mat4 proj;
		Shader basic{ "Shader/test.glsl" };
		//VE::Shader basic{ "Shader/shading.glsl" };

		// post process
		Shader post_process{ "Shader/post_process.glsl" };
		VertexBuffer vbo_post_process;
		VertexArray vao_post_process;

		// Shadows
		Shader shadow_shader{ "Shader/shadow_map.glsl" };
		static constexpr u32 SHADOW_SIZE = 4096;
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

		//skybox
		Shader sky_shader{ "Shader/sky.glsl" };
		Mesh sky_sphere{ Mesh::GenarateSphere() };
	private:
		void RenderShadowMap();
		void RenderGeometry();
		void PostProccess();

	};
}