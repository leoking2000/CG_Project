#include "Sponza.h"

Sponza::Sponza()
	:
GL::Engine(1600, 900, "Computer Graphics Project", true)
{
	//
	// renderer settings
	//
	renderer.light_proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 200.0f);
	renderer.light_view = glm::lookAt(glm::vec3(15.0f, 55.0f, 15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	renderer.light_dir = -glm::normalize(glm::vec3(15.0f, 55.0f, 15.0f));
	renderer.lightSpaceMatrix = renderer.light_proj * renderer.light_view;
	renderer.shadow_bias = 0.000003f;

	renderer.fov_angle = glm::radians(45.0f);
	renderer.min_z = 0.5f;
	renderer.max_z = 100.0f;

	renderer.clear_color = glm::vec3(0.3f, 0.3f, 0.8f);
	renderer.sky_map = "assets/CanopusGround.png";

	//
	// Create the scene
	//
	auto& sponza = GL::ModelManager::GetModel("assets/sponza.obj");

	for (u32 i = 0; i < sponza.meshs.size(); i++)
	{
		sponza.meshs[i].glossiness = 0.0f;
		sponza.meshs[i].MaskMap = "";
		sponza.meshs[i].Lightmap = "";
		sponza.meshs[i].NormalMap = "";
	}

	glm::mat4 model_sponza = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -6.0f, 0.0f));
	model_sponza = glm::scale(model_sponza, glm::vec3(0.02f, 0.02f, 0.02f));

	objects.emplace_back("assets/sponza.obj", model_sponza);

}

Sponza::~Sponza()
{
}

void Sponza::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		f32 dt = ElapsedTime() / 1000.0f;
		cam.Update(win, dt);

		renderer.RenderObjects(win, cam.GetCameraView(), objects);

		EndFrame();

		if (win.KeyIsPress(GLFW_KEY_ESCAPE))
		{
			win.CloseWindow();
		}
	}
}
