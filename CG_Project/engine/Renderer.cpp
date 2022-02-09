#include "graphics/OpenGL.h"
#include "Renderer.h"

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

	void Renderer::RenderTo(Window& win, const glm::mat4& cam_view, const std::vector<GameObject>& gameObjects)
	{
		RenderShadowMap(gameObjects);
		Rendering(win, cam_view, gameObjects);
		PostProcess(win);
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

	void Renderer::PostProcess(Window& win)
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		post_process.Bind();
		vao_post_process.Bind();

		win.fb->BindColorTexture(0);
		post_process.SetUniform("uniform_texture", 0);
		//post_process.SetUniform("uniform_time", continous_time);

		glCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}

}
