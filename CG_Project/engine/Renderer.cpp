#include "graphics/OpenGL.h"
#include "Renderer.h"

#include <ctype.h>

namespace GL
{
	Renderer::Renderer()
	{
		// Initialize post proccess
		f32 fbo_vertices[] = { -1, -1, 1, -1, -1, 1, 1, 1, };
		VertexBuffer vbo_post_process = VertexBuffer(fbo_vertices, sizeof(fbo_vertices));
		ElementType arr[1] = { FLOAT2 };
		Layout<1> layout(arr);
		vao_post_process.AddBuffer(vbo_post_process, layout);
	}

	void Renderer::RenderObjects(Window& win, const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects)
	{
		RenderShadowMap(gameObjects);
		Rendering(win, cam_view, gameObjects);
	}

	void Renderer::RenderShadowMap(const std::vector<GameObject>& gameObjects)
	{
		shadow_map.Bind();
		shadow_shader.Bind();

		glCall(glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE));
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glClear(GL_DEPTH_BUFFER_BIT));

		for (const GameObject& obj : gameObjects)
		{
			if (obj.model_name.empty())
			{
				continue;
			}

			shadow_shader.SetUniform("model", obj.transform);

			ModelManager::GetModel(obj.model_name).Draw(light_view, light_proj, shadow_shader, false);
		}

		shadow_shader.UnBind();
		shadow_map.UnBind();
	}

	void Renderer::Rendering(Window& win, const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects)
	{
		win.fb->Bind();

		glm::vec2 win_size = win.WindowSize();
		glm::mat4 proj = glm::perspective(fov_angle, win_size.x / win_size.y, min_z, max_z);

		glCall(glViewport(0, 0, (int)win_size.x, (int)win_size.y));

		glCall(glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f));
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glDepthFunc(GL_LEQUAL));
		glCall(glClearDepth(1.0f));

		glCall(glEnable(GL_CULL_FACE));
		glCall(glCullFace(GL_BACK));
		glCall(glFrontFace(GL_CCW));

		// skybox
		if (!sky_map.empty())
		{
			glCall(glDepthMask(GL_FALSE));
			glCall(glDisable(GL_CULL_FACE));
			sky_shader.Bind();

			TextureManager::BindTexture(sky_map, 6);
			sky_shader.SetUniform("skybox", 6);

			sky_sphere.Draw(glm::mat4(glm::mat3(cam_view)), proj, sky_shader, false);

			sky_shader.UnBind();
			glCall(glDepthMask(GL_TRUE));
			glCall(glEnable(GL_CULL_FACE));
		}

		// render geometry
		for (const GameObject& obj : gameObjects)
		{
			if (obj.model_name.empty())
			{
				continue;
			}

			rendering_shader.Bind();

			shadow_map.BindDepthTexture(5);
			rendering_shader.SetUniform("shadowMap", 5);
			rendering_shader.SetUniform("bias", shadow_bias);

			if (!sky_map.empty())
			{
				rendering_shader.SetUniform("Has_SkyMap", 1);
				TextureManager::BindTexture(sky_map, 6);
				rendering_shader.SetUniform("skyMap", 6);
			}
			else
			{
				rendering_shader.SetUniform("Has_SkyMap", 0);
			}

			rendering_shader.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			rendering_shader.SetUniform("lightDir", cam_view * glm::vec4(light_dir, 0.0f));

			rendering_shader.SetUniform("model", obj.transform);

			ModelManager::GetModel(obj.model_name).Draw(cam_view, proj, rendering_shader);
		}

		rendering_shader.UnBind();
		win.fb->UnBind();
	}

	void Renderer::RenderText(Window& win, u32 x, u32 y, const std::string& text, f32 scale)
	{
		win.fb->Bind();
		text_shader.Bind();
		font_tex.Bind(7);

		glm::vec2 win_size = win.WindowSize();

		u32 curX = x;
		u32 curY = win_size.y - y - scale;

		for (u32 i = 0; i < text.size(); i++)
		{
			if (text[i] == '\n')
			{
				curX = x;
				curY -= scale;
				continue;
			}

			char c = text[i];

			RenderChar(win, curX, curY, c, scale);
			curX += scale;
		}

		text_shader.UnBind();
		win.fb->UnBind();
	}

	void Renderer::RenderChar(Window& win, u32 x, u32 y, char c, f32 scale)
	{
		// find tex coords
		glm::vec2 coords;
		if (!GetCharacterTexCoords(c, coords))
		{
			return;
		}

		// projection
		glm::vec2 win_size = win.WindowSize();
		glm::mat4 proj = glm::ortho(0.0f, win_size.x, 0.0f, win_size.y);

		glm::vec2 coord0 = normalizeTexCoords(coords);
		glm::vec2 coord1 = normalizeTexCoords({ coords.x + glyphWidth, coords.y });
		glm::vec2 coord2 = normalizeTexCoords({ coords.x , coords.y - glyphHeight });
		glm::vec2 coord3 = normalizeTexCoords({ coords.x + glyphWidth, coords.y - glyphHeight });

		float vertexs[] = {
			x        , y        , coord2.x, coord2.y,
			x + scale, y        , coord3.x, coord3.y,
			x        , y + scale, coord0.x, coord0.y,

			x + scale, y        , coord3.x, coord3.y,
			x + scale, y + scale, coord1.x, coord1.y,
			x        , y + scale, coord0.x, coord0.y
		};

		VertexBuffer vertexBuffer(vertexs, sizeof(vertexs));

		ElementType arr[2] = { FLOAT2, FLOAT2 };
		Layout<2> layout(arr);

		VertexArray vertexArray;
		vertexArray.AddBuffer(vertexBuffer, layout);

		vertexArray.Bind();

		text_shader.SetUniform("proj", proj);
		text_shader.SetUniform("font", 7);
		text_shader.SetUniform("color", glm::vec3(0.7f, 0.7f, 0.8f));

		glCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	}

	glm::vec2 Renderer::normalizeTexCoords(const glm::vec2& coord)
	{
		return glm::vec2(coord.x / font_width, coord.y / font_height);
	}

	bool Renderer::GetCharacterTexCoords(char c, glm::vec2& coord)
	{
		if (c >= firstChar && c <= lastChar)
		{
			u32 glyphIndex = c - firstChar;
			u32 yGlyph = glyphIndex / nColumns;
			u32 xGlyph = glyphIndex % nColumns;

			coord = glm::vec2(xGlyph * glyphWidth, font_height - (yGlyph * glyphHeight));

			return true;
		}

		return false;
	}

	void Renderer::RenderToScreen(Window& win, float continous_time)
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		post_process.Bind();
		vao_post_process.Bind();

		win.fb->BindColorTexture(0);
		post_process.SetUniform("uniform_texture", 0);
		post_process.SetUniform("uniform_time", continous_time);

		glCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

		post_process.UnBind();
		vao_post_process.UnBind();
	}



}
