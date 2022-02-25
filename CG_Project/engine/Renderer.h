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
		void RenderObjects(Window& win,const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects);
		void RenderToScreen(Window& win, float continous_time); // post proccess
		void RenderText(Window& win, u32 x, u32 y, const std::string& text, u32 scale = 32);
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
	private:
		// text rendering
		Texture font_tex{ "Shader/Consolas13x24.bmp", true };
		static constexpr u32 font_width = 416;
		static constexpr u32 font_height = 72;

		Shader text_shader{ "Shader/text.glsl" };

		static constexpr u32 glyphWidth = 13;
		static constexpr u32 glyphHeight = 24;
		static constexpr char firstChar = ' ';
		static constexpr char lastChar = '~';
		static constexpr int nColumns = 32;
		static constexpr int nRows = 3;

		// [0, w-h] ---> [0, 1]
		glm::vec2 normalizeTexCoords(const glm::vec2& coord);

		void RenderChar(Window& win, u32 x, u32 y, char c, u32 scale);

		// returns true if c is printable.
		bool GetCharacterTexCoords(char c, glm::vec2& coord);
	};
}



