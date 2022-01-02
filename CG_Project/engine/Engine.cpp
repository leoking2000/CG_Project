#include "Engine.h"
#include "log.h"

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace GL
{
	Engine::Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable)
		:
		win(window_width, window_height, window_title, resizable),
		vbo_post_process(nullptr, 0),
		lightSpaceMatrix(light_proj * light_view)
	{
		// Initialize ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(win.glfw_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		// Initialize post proccess
		GLfloat fbo_vertices[] = { -1, -1, 1, -1, -1, 1, 1, 1, };
		vbo_post_process = VertexBuffer(fbo_vertices, sizeof(fbo_vertices));
		ElementType arr[1] = { FLOAT2 };
		Layout<1> layout(arr);
		vao_post_process.AddBuffer(vbo_post_process, layout);
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
		RenderShadowMap();
		RenderGeometry();
		PostProccess();

		previousTime = currentTime;
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(win.glfw_window);
	}

	void Engine::RenderShadowMap()
	{
		shadow_map.Bind();

		glCall(glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE));
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glClear(GL_DEPTH_BUFFER_BIT));

		shadow_shader.Bind();

		for (GameObject& obj : objets)
		{
			if (obj.model_name.empty())
			{
				continue;
			}

			shadow_shader.SetUniform("model", obj.transform);

			ModelManager::GetModel(obj.model_name).Draw(light_view, light_proj, shadow_shader, false);
		}
	}

	void Engine::RenderGeometry()
	{
		win.fb->Bind();

		glm::vec2 win_size = win.WindowSize();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), win_size.x / win_size.y, 0.001f, 1000.0f);
		glm::mat4 cam_view = cam.GetCameraView();

		glCall(glViewport(0, 0, (int)win_size.x, (int)win_size.y));

		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glDepthFunc(GL_LEQUAL));
		glCall(glClearDepth(1.f));

		glCall(glEnable(GL_CULL_FACE));
		glCall(glCullFace(GL_BACK));
		glCall(glFrontFace(GL_CCW));

		glCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// skybox
		glCall(glDepthMask(GL_FALSE));
		glCall(glDisable(GL_CULL_FACE));

		sky_shader.Bind();
		TextureManager::GetTexture(sky_map).Bind(0);
		sky_shader.SetUniform("skybox", 0);

		sky_sphere.Draw(glm::mat4(glm::mat3(cam_view)), proj, sky_shader, false);

		glCall(glDepthMask(GL_TRUE));
		glCall(glEnable(GL_CULL_FACE));

		for (GameObject& obj : objets)
		{
			if (obj.model_name.empty())
			{
				continue;
			}

			basic.Bind();

			shadow_map.BindDepthTexture(5);
			basic.SetUniform("shadowMap", 5);
			basic.SetUniform("bias", 0.0000005f);

			basic.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			basic.SetUniform("lightDir", cam_view * glm::vec4(light_dir, 0.0f));

			basic.SetUniform("model", obj.transform);

			ModelManager::GetModel(obj.model_name).Draw(cam_view, proj, basic);
		}

	}

	void Engine::PostProccess()
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		post_process.Bind();
		vao_post_process.Bind();

		win.fb->BindColorTexture(0);
		//shadow_map.BindDepthTexture(0);
		post_process.SetUniform("uniform_texture", 0);

		//post_process.SetUniform("uniform_time", continous_time);

		glCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}


}


