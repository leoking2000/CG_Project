#pragma once
#include <chrono>
#include "OpenGL.h"
#include "Window.h"
#include "../imgui/imgui.h"

#include "log.h"
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "FrameBuffer.h"

namespace VE
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
		std::vector<Model> models;
		VE::Camera cam;

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
		FrameBuffer core_frame_buffer;
		glm::mat4 proj;
		VE::Shader basic{ "Shader/shading.glsl" };

		// post process
		Shader post_process{ "Shader/post_process.glsl" };
		VertexBuffer vbo_post_process;
		VertexArray vao_post_process;

		// Shadows
		VE::Shader shadow_shader{ "Shader/shadow_map.glsl" };
		static constexpr u32 SHADOW_SIZE = 4096;
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

		//skybox
		VE::Shader sky_shader{ "Shader/sky.glsl" };
		VE::Mesh sky_sphere{ VE::Mesh::GenarateSphere() };

	private:
		void RenderShadowMap();
		void RenderGeometry();
		void PostProccess();

	};
}