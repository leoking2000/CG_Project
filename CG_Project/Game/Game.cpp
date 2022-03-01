#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Code along Project", true)
{
	//
	// renderer settings
	//
	renderer.light_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1000.0f);
	renderer.light_view = glm::lookAt(glm::vec3(0.0f, 450.0f, -150.0f), glm::vec3(0.0f, 0.0f, -130.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	renderer.light_dir = -glm::normalize(glm::vec3(0.0f, 500.0f, -100.0f));
	renderer.lightSpaceMatrix = renderer.light_proj * renderer.light_view;
	renderer.shadow_bias = 0.0000005f;

	renderer.fov_angle = glm::radians(45.0f);
	renderer.min_z = 0.5f;
	renderer.max_z = 800.0f;

	renderer.clear_color = glm::vec3(0.0f, 0.0f, 0.0f);

	//
	// Create the scene
	//

}

Game::~Game()
{
	GL::LogInfo("Destroyed Game");
}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		//glm::vec2 win_size = win.WindowSize();
		//renderer.RenderObjects(win, cam.GetCameraView(), objects);

		renderer.RenderText(win, 0, 0, "Code along!!!", 40);



	
		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}

