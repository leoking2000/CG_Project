#include "Engine.h"

class Game : public VE::Engine
{
private:

public:
	Game()
		:
	VE::Engine(1600, 900, "Computer Graphics Project", true, std::vector<std::string>() )
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

			ImGui::Begin("settings");
			ImGui::Text("\nlight settings");

			ImGui::SliderFloat("Light pos X ", &light.position.x, -100.0f, 100.0f);
			ImGui::SliderFloat("light pos Y ", &light.position.y,  100.0f, 500.0f);
			ImGui::SliderFloat("light pos Z ", &light.position.z, -100.0f, 100.0f);

			ImGui::ColorEdit3("light color", glm::value_ptr(light.color));
			ImGui::InputFloat("quadratic term ", &light.quadratic_term, 0.001f, 0.1f);
			ImGui::InputFloat("linear term ", &light.linear_term, 0.001f, 0.1f);
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