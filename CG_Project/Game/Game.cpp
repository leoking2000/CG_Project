#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Computer Graphics Project", true),
collision_hull(GL::ObjLoader::Load("assets/collision_hull.obj")[0])
{
	sky_map = "assets/CanopusGround.png";

	// make the red sphere model 
	GL::ModelManager::Make("sphere", GL::GenarateSphere()).meshs[0].defaultColor = glm::vec3(1.0f, 0.0f, 0.0f);

	// make the craft
	objets.emplace_back("assets/craft.obj", glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up)));

	// make the terrain
	objets.emplace_back("assets/terrain.obj", glm::mat4(1.0f));
	GL::ModelManager::GetModel("assets/terrain.obj").meshs[0].Lightmap = "assets/terrain_Lightmap.png";

	// make some plants
	glm::mat4 model_plant = glm::translate(glm::mat4(1.0f), glm::vec3(-130.0f, 0.0f, 80.0f));
	model_plant = glm::scale(model_plant, glm::vec3(0.2f, 0.2f, 0.2f));
	objets.emplace_back("assets/hoewa_Forsteriana_1.obj", model_plant);

	model_plant = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	model_plant = glm::scale(model_plant, glm::vec3(0.5f, 0.5f, 0.5f));
	objets.emplace_back("assets/hoewa_Forsteriana_1.obj", model_plant);

	// make sphere game objects
	objets.emplace_back("sphere", glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 50.0f, 0.0f)));

	cam.pos = glm::vec3(0.0f, 60.0f, 0.0f);
}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		DebugUpdate();
		//Update();


		ImGui::Begin("FPS");
		ImGui::Text("ElapsedTime: %f ms", ElapsedTime());
		ImGui::Text("FPS: %f\n", glm::round(1 / (ElapsedTime() / 1000.0f)));
		ImGui::End();
	
		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}

void Game::DebugUpdate()
{
	f32 dt = ElapsedTime() / 1000.0f;

	if (win.MouseButtonIsPress(GLFW_MOUSE_BUTTON_1))
	{
		dt = ElapsedTime() / 100.0f;
	}

	cam.Update(win, dt);

	if (win.KeyIsPress(GLFW_KEY_R))
	{
		reloadShaders();
	}
}

void Game::Update()
{
	MoveCraft();

	cam.pos = objets[0].transform * rel_cam_pos;
	cam.dir = craft_facing;

	f32 d;
	bool collition = collision_hull.intersectRay_Local(craft_pos, craft_facing, d);

	if (collition && d < 500.0f)
	{
		craft_pos = glm::vec3(-100.0f, 50.0f, 75.0f);
		craft_facing = glm::vec3(0.0f, 0.0f, -1.0f);
		craft_right = glm::vec3(1.0f, 0.0f, 0.0f);
		craft_up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	ImGui::Begin("Collition");
	ImGui::Text("collition: %s", collition ? "true" : "false");
	ImGui::Text("d: %f", d);
	ImGui::End();
}

void Game::MoveCraft()
{
	f32 dt = ElapsedTime() / 1000.0f;

	if (dt > 0.016f) dt = 0.016;

	if (win.MouseButtonIsPress(GLFW_MOUSE_BUTTON_1))
	{
		glm::vec2 input = Input();

		craft_facing = glm::normalize(craft_facing + input.x * craft_right * dt + input.y * craft_up * dt);
		craft_right = glm::normalize(glm::cross(craft_facing, glm::vec3(0.0f, 1.0f, 0.0f)));
		craft_up = glm::cross(craft_right, craft_facing);	
	}

	if (win.MouseButtonIsPress(GLFW_MOUSE_BUTTON_2) || true)
	{
		craft_pos = craft_pos + speed * craft_facing * dt;
	}

	objets[0].transform = glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up));
}

glm::vec2 Game::Input()
{
	constexpr f32 a = 2.0f;

	glm::vec2 mouse = win.MousePos();
	glm::vec2 size = win.WindowSize();

	f32 x = -a + (2 * a / size.x) * mouse.x;
	f32 y = -a + (2 * a / size.y) * mouse.y;

	return glm::vec2(x, -y);
}

Game::~Game()
{
	GL::LogInfo("Destroyed Game");
}


