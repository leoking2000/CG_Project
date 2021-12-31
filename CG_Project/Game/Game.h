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

		models.emplace_back("assets/terrain.obj");
		models[1].m_mesh[0].Lightmap = "assets/terrain_Lightmap.png";


		cam.pos = glm::vec3(0.0f, 60.0f, 0.0f);
	}

	void Start()
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

			//cam.Update(win, ElapsedTime() / 100.0f);
			MoveCraft(ElapsedTime() / 1000.0f);

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
private:
	glm::vec3 craft_pos = glm::vec3(-100.0f, 50.0f, 75.0f);
	glm::vec3 offset = glm::vec3(0.0f, 0.0f, -100.0f);
	glm::vec3 rel_pos = glm::vec3(0.0f, 9.0f, 25.0f);

private:
	void MoveCraft(float dt)
	{

		


		// update craft model matrix
		glm::mat4 model_craft = glm::translate(glm::mat4(1.0f), craft_pos);
		model_craft = glm::scale(model_craft, glm::vec3(1.0f, 1.0f, 1.0f));
		models[0].transform = model_craft;

		// update camera pos
		cam.pos = craft_pos + rel_pos;
		glm::vec4 target = model_craft * glm::vec4(offset, 1.0f);
		cam.dir = glm::normalize(glm::vec3(target) - cam.pos);
	}



};

