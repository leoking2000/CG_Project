#pragma once
#include <vector> 
#include "graphics/Window.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "Model.h"

#include "GameObject.h"

namespace GL
{
	class Renderer
	{
	public:
		Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
	public:
		void RenderTo(Window& win,const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects);
	public:
		// light
		glm::mat4 light_proj;
		glm::mat4 light_view;
		glm::vec3 light_dir;
		glm::mat4 lightSpaceMatrix;
		f32 shadow_bias = 0.005f;

		// projection settings
		f32 fov_angle;
		f32 min_z;
		f32 max_z;

		// clear color
		glm::vec3 clear_color;
		std::string sky_map;

	private:
		// shadow map
		static constexpr u32 SHADOW_SIZE = 4096;
		Shader shadow_shader{ "Shader/shadow_map.glsl" };
		FrameBuffer shadow_map{ SHADOW_SIZE, SHADOW_SIZE, true };

		void RenderShadowMap(const std::vector<GameObject>& gameObjects);
	private:
		// Rendering
		Shader rendering_shader{ "Shader/rendering.glsl" };

		//skybox
		Shader sky_shader{ "Shader/sky.glsl" };
		Mesh sky_sphere{ GenarateSphere() };

		void Rendering(Window& win, const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects);
	private:
		// post process
		Shader post_process{ "Shader/post_process.glsl" };
		VertexArray vao_post_process;

		void PostProcess(Window& win);
	};
}



