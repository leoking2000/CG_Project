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
#include "light/PointLight.h"

namespace VE
{
	class Engine
	{
	public:
		Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable, std::vector<std::string>&& skybox);

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

		VE::Camera cam;

		// light
		VE::PointLight light{ { 0.0f, 500.0f, -100.0f } };
		float bias = 0.0f;

		std::vector<Model> models;

	private:
		std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::time_point previousTime = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds elapsed;
		f32 continous_time = 0;
	private:
		FrameBuffer core_frame_buffer;
		glm::mat4 proj;
		VE::Shader basic{ "Shader/basicTex.glsl" };
		VE::Shader light_shader{ "Shader/light.glsl" };

		// post process
		Shader post_process{ "Shader/post_process.glsl" };
		VertexBuffer vbo_post_process;
		VertexArray vao_post_process;

		// skybox
		VE::Shader skybox_shader{ "Shader/skybox.glsl" };
		VE::Mesh skybox_mesh{ VE::Mesh::Skybox() };
		VE::CubeMap skybox_cubemap;

		// Shadows
		VE::Shader shadow_shader{ "Shader/shadow_map.glsl" };
		static constexpr u32 SHADOW_SIZE = 4096;
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

	private:
		void RenderShadowMap();
		void RenderGeometry();
		void PostProccess();

	};
}