#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Computer Graphics Project", true)
{
	sky_map = "assets/CanopusGround.png";

	// cashing
	GL::ModelManager::GetModel("assets/terrain.obj").m_mesh[0].Lightmap = "assets/terrain_Lightmap.png";
	GL::ModelManager::GetModel("assets/craft.obj");
	GL::ModelManager::GetModel("assets/hoewa_Forsteriana_1.obj");

	// make the craft
	glm::mat4 model_craft = glm::translate(glm::mat4(1.0f), craft_pos);
	model_craft = glm::scale(model_craft, glm::vec3(1.0f, 1.0f, 1.0f));
	objets.emplace_back("assets/craft.obj", model_craft);

	// make a game object for the terrain
	objets.emplace_back("assets/terrain.obj", glm::mat4(1.0f));

	// make some plants
	glm::mat4 model_plant = glm::translate(glm::mat4(1.0f), glm::vec3(-130.0f, 0.0f, 80.0f));
	model_plant = glm::scale(model_plant, glm::vec3(0.2f, 0.2f, 0.2f));
	objets.emplace_back("assets/hoewa_Forsteriana_1.obj", model_plant);

	model_plant = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	model_plant = glm::scale(model_plant, glm::vec3(0.5f, 0.5f, 0.5f));
	objets.emplace_back("assets/hoewa_Forsteriana_1.obj", model_plant);

	cam.pos = glm::vec3(0.0f, 60.0f, 0.0f);
}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		ImGui::Begin("FPS");
		ImGui::Text("ElapsedTime: %f ms", ElapsedTime());
		ImGui::Text("FPS: %f\n", std::round(1 / (ElapsedTime() / 1000.0f)));
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