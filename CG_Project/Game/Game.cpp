#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Computer Graphics Project", true)
{
	sky_map = "assets/CanopusGround.png";

	models.emplace_back("assets/craft.obj");
	glm::mat4 model_craft = glm::translate(glm::mat4(1.0f), craft_pos);
	model_craft = glm::scale(model_craft, glm::vec3(1.0f, 1.0f, 1.0f));
	models[0].transform = model_craft;

	models.emplace_back("assets/terrain.obj");
	models[1].m_mesh[0].Lightmap = "assets/terrain_Lightmap.png";


	cam.pos = glm::vec3(0.0f, 60.0f, 0.0f);
}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		glm::vec2 mouse = win.MousePos();

		NewFrame();

		ImGui::Begin("FPS");
		ImGui::Text("ElapsedTime: %f ms", ElapsedTime());
		ImGui::Text("FPS: %f\n", std::round(1 / (ElapsedTime() / 1000.0f)));

		ImGui::Text("Mouse X: %f | Y: %f ", mouse.x, mouse.y);

		ImGui::End();

		cam.Update(win, ElapsedTime() / 100.0f);

		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}

Game::~Game()
{
	LogInfo("Destroyed Game");
}