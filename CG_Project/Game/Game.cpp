#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Computer Graphics Project", true),
collision_hull(GL::ObjLoader::Load("assets/collision_hull.obj")[0])
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

	renderer.clear_color = glm::vec3(0.3f, 0.3f, 0.8f);
	renderer.sky_map = "assets/CanopusGround.png";

	//
	// Create the scene
	//

	// make the sphere model for the score points
	GL::ModelManager::Make("sphere", GL::GenarateSphere()).meshs[0].defaultColor = glm::vec3(0.0f, 0.0f, 1.0f);

	// make Phormium_1 model
	GL::ModelManager::GetModel("assets/Phormium_1.obj").meshs[0].glossiness = 0.0f;

	// make the craft
	objects.emplace_back("assets/craft.obj", glm::mat4(1.0f));
	Reset();

	// make on "broken" craft
	objects.emplace_back("assets/craft.obj", 
		glm::translate(glm::vec3(-230.0f, 5.0f, -180.0f)) *
		glm::rotate(glm::mat4(1.0f), PI / 6.0f + 0.06f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), -PI / 6.0f, glm::vec3(0.0f, 0.0f, 1.0f)));

	// make the terrain
	objects.emplace_back("assets/terrain.obj", glm::mat4(1.0f));
	GL::ModelManager::GetModel("assets/terrain.obj").meshs[0].Lightmap = "assets/terrain_Lightmap.png";

	// make the grass
	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(-70.0f, -2.0f, -10.0f)) *
		glm::scale(glm::vec3(2.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3( 10.0f, -2.0f, -40.0f)) *
		glm::scale(glm::vec3(2.0f))
	);

	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(75.0f, 3.0f, -40.0f)) *
		glm::scale(glm::vec3(1.5f)) *
		glm::rotate(glm::mat4(1.0f), PI / 6.0f, glm::vec3(0.0f, 0.0f, 1.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(100.0f, 0.0f, 70.0f)) *
		glm::scale(glm::vec3(2.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(130.0f, 5.0f, 145.0f)) *
		glm::scale(glm::vec3(2.0f)) *
		glm::rotate(glm::mat4(1.0f), PI / 6.0f, glm::vec3(0.0f, 0.0f, 1.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-140.0f, -2.0f, 100.0f)) *
		glm::scale(glm::vec3(3.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-160.0f, 3.0f, -30.0f)) *
		glm::scale(glm::vec3(2.0f)) *
		glm::rotate(glm::mat4(1.0f), PI / 6.0f, glm::vec3(0.0f, 0.0f, 1.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-280.0f, 5.0f, 10.0f)) *
		glm::scale(glm::vec3(4.0f)) *
		glm::rotate(glm::mat4(1.0f), -PI / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f))
	);

	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(-40.0f, 5.0f, -90.0f)) *
		glm::scale(glm::vec3(2.0f)) *
		glm::rotate(glm::mat4(1.0f), PI / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-160.0f, 2.0f, -160.0f)) *
		glm::scale(glm::vec3(5.0f))
	);

	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(-230.0f, 1.5f, -250.0f)) *
		glm::scale(glm::vec3(1.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(0.0f, -3.0f, -250.0f)) *
		glm::scale(glm::vec3(2.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-50.0f, 2.0f, -320.0f)) *
		glm::scale(glm::vec3(2.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(-100.0f, 2.0f, -270.0f)) *
		glm::scale(glm::vec3(3.0f))
	);

	objects.emplace_back("assets/Phormium_1.obj",
		glm::translate(glm::vec3(140.0f, 5.0f, -350.0f)) * 
		glm::scale(glm::vec3(3.0f))
	);

	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(170.0f, 5.0f, -160.0f)) *
		glm::scale(glm::vec3(3.0f))
	);

	objects.emplace_back("assets/Phormium_3.obj",
		glm::translate(glm::vec3(200.0f, 5.0f, -300.0f)) *
		glm::scale(glm::vec3(3.0f))
	);

	// make sphere game objects
	score_points.emplace_back(glm::vec3(    0.0f,  25.0f, -200.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  150.0f,  25.0f, -230.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  140.0f,  30.0f, -350.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  -10.0f,  50.0f, -300.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3( -140.0f,  55.0f, -240.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3( -140.0f,  30.0f, -110.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3( -200.0f,  25.0f,  -10.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3( -100.0f,  15.0f,   80.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  -80.0f,  40.0f,  -30.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(   10.0f,  40.0f,  -10.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  100.0f,  60.0f,   60.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  210.0f,  50.0f,   10.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  145.0f,  40.0f, -130.0f ), "sphere", objects);
	score_points.emplace_back(glm::vec3(  250.0f,  60.0f, -130.0f ), "sphere", objects);

	cam.pos = objects[0].transform * glm::vec4(20.0f, 5.0f, -20.0f, 1.0f);
	cam.dir = glm::normalize(craft_pos - cam.pos);
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

		std::string msg;

		switch (state)
		{
		case START:
			DebugUpdate();
			if (win.KeyIsPress(GLFW_KEY_ENTER))
			{
				state = PLAY;
				win.SetMouseVisibility(true);
			}
			msg = "Press ENTER to Play!!!";
			break;
		case PLAY:
			GameUpdate();
			break;
		case GAMEOVER:
			if (win.KeyIsPress(GLFW_KEY_ENTER))
			{
				Reset();
				state = PLAY;
			}
			msg = "GAME OVER!!! \nPress ENTER to Play Again!!!";
			break;
		}

		ImGui::Begin("Score");
		ImGui::Text("Total Score: %i", score);
		ImGui::Text("current speed %f", craft_speed);
		ImGui::Text(msg.c_str());
		ImGui::End();

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

void Game::Reset()
{
	score = 0;

	craft_speed = 80.0f;
	craft_pos = glm::vec3(-80.0f, 40.0f, 150.0f);
	craft_facing = glm::vec3(0.0f, 0.0f, -1.0f);
	craft_right = glm::vec3(1.0f, 0.0f, 0.0f);
	craft_up = glm::vec3(0.0f, 1.0f, 0.0f);
	objects[0].transform = glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up));
}

void Game::DebugUpdate()
{
	f32 dt = ElapsedTime() / 100.0f;

	cam.Update(win, dt);
}

void Game::GameUpdate()
{
	MoveCraft();

	cam.pos = glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up)) * rel_cam_pos;
	cam.dir = craft_facing;

	f32 d1, d2, d3;
	bool collition1 = collision_hull.intersectRay_Local(craft_pos, craft_facing, d1);
	bool collition2 = collision_hull.intersectRay_Local(craft_pos + glm::vec3(3.0f, 0.0f, 0.0f), craft_facing, d2);
	bool collition3 = collision_hull.intersectRay_Local(craft_pos + glm::vec3(-3.0f, 0.0f, 0.0f), craft_facing, d3);

	if ((collition1 && d1 < 500) || (collition2 && d2 < 500) || (collition3 && d3 < 500))
	{
		state = GAMEOVER;
	}

	for (ScorePoint& sp : score_points)
	{
		sp.Tick(craft_pos, ElapsedTime(), 
			[&]() -> void
			{
				score += 10;
				craft_speed += 1;
			},
			objects
		);
	}

}

void Game::MoveCraft()
{
	f32 dt = ElapsedTime() / 1000.0f;

	if (dt > 0.016f) dt = 0.016f;

	glm::vec2 input = Input();
	glm::mat4 rotate = glm::mat4(1.0f);

	if (win.MouseButtonIsPress(GLFW_MOUSE_BUTTON_1))
	{
		craft_facing = glm::normalize(craft_facing + input.x * craft_right * dt + input.y * craft_up * dt);
		craft_right = glm::normalize(glm::cross(craft_facing, glm::vec3(0.0f, 1.0f, 0.0f)));
		craft_up = glm::cross(craft_right, craft_facing);

		rotate = glm::rotate(glm::mat4(1.0f), -(PI / 4.0f) * input.x, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	craft_pos = craft_pos + craft_speed * craft_facing * dt;

	if (craft_pos.y > max_y)
	{
		craft_pos.y = max_y;

		craft_facing = glm::normalize(craft_facing - craft_up * dt);
		craft_right = glm::normalize(glm::cross(craft_facing, glm::vec3(0.0f, 1.0f, 0.0f)));
		craft_up = glm::cross(craft_right, craft_facing);
	}

	objects[0].transform = glm::inverse(glm::lookAt(craft_pos, craft_pos + craft_facing, craft_up)) * rotate;

}



void Game::OnCollision_ScorePoint()
{
	score += 10;
	craft_speed += 1;
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


