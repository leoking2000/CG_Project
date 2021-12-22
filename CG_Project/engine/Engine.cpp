#include "Engine.h"
#include "log.h"

#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

namespace VE
{
	Engine::Engine(u32 window_width, u32 window_height, const char* window_title, bool resizable, std::vector<std::string>&& skybox)
		:
		win(window_width, window_height, window_title, resizable),
		core_frame_buffer(window_width, window_height),
		vbo_post_process(nullptr, 0)
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

		// Initialize skybox
		if (skybox.size() != 0)
		{
			skybox_cubemap.reload(skybox);
		}
	}

	Engine::~Engine()
	{
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

		glm::mat4 l_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
		glm::mat4 l_view = glm::lookAt(light.position, light.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		shadow_shader.Bind();

		for (Model& obj : models)
		{
			obj.Draw(l_view, l_proj, shadow_shader, false);
		}
	}

	void Engine::RenderGeometry()
	{
		core_frame_buffer.Bind();

		glm::vec2 win_size = win.WindowSize();
		proj = glm::perspective(glm::radians(45.0f), win_size.x / win_size.y, 0.1f, 1000.0f);
		glCall(glViewport(0, 0, (int)win_size.x, (int)win_size.y));

		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glDepthFunc(GL_LEQUAL));
		glCall(glClearDepth(1.f));

		glCall(glEnable(GL_CULL_FACE));
		glCall(glCullFace(GL_BACK));
		glCall(glFrontFace(GL_CCW));

		glCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		if (skybox_cubemap.isInitialized())
		{
			glCall(glDepthMask(GL_FALSE));

			skybox_shader.Bind();
			skybox_cubemap.Bind(0);
			skybox_shader.SetUniform("skybox", 0);

			skybox_mesh.Draw(glm::mat4(glm::mat3(cam.GetCameraView())), proj, skybox_shader, false);

			glCall(glDepthMask(GL_TRUE));
		}

		glm::mat4 l_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
		glm::mat4 l_view = glm::lookAt(light.position, light.position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 lightSpaceMatrix = l_proj * l_view;

		for (Model& obj : models)
		{
			basic.Bind();

			shadow_map.BindDepthTexture(5);
			basic.SetUniform("shadowMap", 5);
			basic.SetUniform("lightSpaceMatrix", lightSpaceMatrix);
			basic.SetUniform("u_light_pos", light.position);
			basic.SetUniform("light.light_color", light.color);
			basic.SetUniform("light.quadratic_term", light.quadratic_term);
			basic.SetUniform("light.linear_term", light.linear_term);

			obj.Draw(cam.GetCameraView(), proj, basic);
		}

		light.Draw(cam.GetCameraView(), proj, light_shader);
	}

	void Engine::PostProccess()
	{
		glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		post_process.Bind();
		vao_post_process.Bind();

		core_frame_buffer.BindColorTexture(0);
		post_process.SetUniform("uniform_texture", 0);

		//post_process.SetUniform("uniform_time", continous_time);

		glCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
	}


}


