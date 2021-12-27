#include "Engine.h"

class Game : public VE::Engine
{
public:
	Game()
		:
	VE::Engine(1600, 900, "Computer Graphics Project", true)
	{
		sky_map = "assets/CanopusGround.png";

		models.emplace_back("assets/craft.obj");
		glm::mat4 model_craft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f));
		model_craft = glm::scale(model_craft, glm::vec3(2.0f, 2.0f, 2.0f));
		models[0].transform = model_craft;

		models.emplace_back("assets/terrain.obj");


		cam.pos = glm::vec3(0.0f, 60.0f, 0.0f);
	}

	void Start()
	{
		while (!win.ShouldWindowClose())
		{
			NewFrame();

			ImGui::Begin("FPS");
			ImGui::Text("ElapsedTime: %f ms", ElapsedTime());
			ImGui::Text("FPS: %f\n", std::round(1 / (ElapsedTime() / 1000.0f)));

			ImGui::Text("X: %f | Y: %f | Z: %f", cam.pos.x, cam.pos.y, cam.pos.z);

			ImGui::End();

			cam.Update(win, ElapsedTime() / 100.0f);

			EndFrame();

			if (win.KeyIsPress(GLFW_KEY_ESCAPE))
			{
				win.CloseWindow();
			}
		}
	}

	~Game()
	{
		LogInfo("Destroyed Game");
	}

};

