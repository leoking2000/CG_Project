#include "Engine.h"

class Game : public VE::Engine
{
private:

public:
	Game()
		:
	VE::Engine(1600, 900, "Computer Graphics Project", true)
	{
		//light.position = glm::vec3(0.0f, 150.0f, 0.0f);
		//light.quadratic_term = 0.0f;

		models.emplace_back("assets/craft.obj");
		glm::mat4 model_craft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 50.0f, 0.0f));
		models[0].transform = model_craft;

		models.emplace_back("assets/terrain.obj");


		cam.pos = glm::vec3(10.0f, 30.0f, 0.0f);
	}

	void Start()
	{
		while (!win.ShouldWindowClose())
		{
			NewFrame();

			ImGui::Begin("FPS");
			ImGui::Text("ElapsedTime: %f ms", ElapsedTime());
			ImGui::Text("FPS: %f", std::round(1 / (ElapsedTime() / 1000.0f)));
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

