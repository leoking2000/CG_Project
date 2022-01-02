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
	objets.emplace_back("assets/craft.obj", glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up)));

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

		Update();

		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}

void Game::Update()
{
	if (win.MouseButtonIsPress(GLFW_MOUSE_BUTTON_1))
	{
		MoveCraft();
	}

	//cam.Update(win, ElapsedTime() / 100.0f);

	cam.pos = objets[0].transform * rel_cam_pos;
	cam.dir = craft_facing;
}

void Game::MoveCraft()
{
	f32 dt = ElapsedTime() / 1000.0f;

	glm::vec2 input = Input();

	craft_facing = glm::normalize(craft_facing + input.x * craft_right * dt + input.y * craft_up * dt);
	craft_right = glm::normalize(glm::cross(craft_facing, glm::vec3(0.0f, 1.0f, 0.0f)));
	craft_up = glm::cross(craft_right, craft_facing);

	craft_pos = craft_pos + speed * craft_facing * dt;

	objets[0].transform = glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up));
}

glm::vec2 Game::Input()
{
	constexpr f32 a = 1.0f;

	glm::vec2 mouse = win.MousePos();
	glm::vec2 size = win.WindowSize();

	f32 x = -a + (2 * a / size.x) * mouse.x;
	f32 y = -a + (2 * a / size.y) * mouse.y;

	return glm::vec2(x, -y);
}

Game::~Game()
{
	LogInfo("Destroyed Game");
}


