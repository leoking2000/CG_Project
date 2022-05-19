#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "CG Project", true)
{
	//
	// renderer settings
	//
	renderer.light_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	renderer.light_view = glm::lookAt(glm::vec3(0.0f, 25.0f, -25.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	renderer.light_dir = -glm::normalize(glm::vec3(0.0f, 25.0f, -25.0f));
	renderer.lightSpaceMatrix = renderer.light_proj * renderer.light_view;
	renderer.shadow_bias = 0.00005f;

	renderer.fov_angle = glm::radians(45.0f);
	renderer.min_z = 0.5f;
	renderer.max_z = 800.0f;

	renderer.clear_color = glm::vec3(0.3f, 0.3f, 0.8f);

	//
	// Set up game
	//

	GL::ModelManager::Make("sphere", GL::GenarateSphere()).meshs[0].defaultColor = glm::vec3(0.0f, 0.0f, 1.0f);
	GL::ModelManager::Make("plane", GL::GenarateSphere()).meshs[0].defaultColor = glm::vec3(1.0f, 1.0f, 1.0f);

	GL::ModelManager::GetModel("sphere").meshs[0].glossiness = 0.0f;
	GL::ModelManager::GetModel("sphere").meshs[0].metallic = 0.0f;

	objects.emplace_back("sphere", glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)));

	objects.emplace_back("plane", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -10.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(20.0f, 1.0f, 20.0f)));

	cam.pos = glm::vec3(0.0f, 0.0f, 5.0f);

}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		f32 dt = ElapsedTime() / 1000.0f;
		cam.Update(win, dt);

		renderer.RenderObjects(win, cam.GetCameraView(), objects);
		renderer.RenderText(win, 0, 0, 
			"X " + std::to_string(cam.pos.x) + "\n" +
			"Y " + std::to_string(cam.pos.y) + "\n" + 
			"Z " + std::to_string(cam.pos.z) + "\n"
			, 40);

		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}

Game::~Game()
{
	GL::LogInfo("Destroyed Game");
}

