#include "DirectionalLight.h"

namespace VE
{
	DirectionalLight::DirectionalLight(const glm::vec3 dir)
		:
		direction(glm::normalize(dir))
	{
		//proj = glm::ortho(-10, 10, -10, 10, 1, 10);
		proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
		view = glm::lookAt(glm::vec3(-7.0f, 55.0f, -18.0f), glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void DirectionalLight::RenderShadowMap(const std::vector<Model>& models)
	{
		shadow_map.Bind();

		glCall(glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE));
		glCall(glEnable(GL_DEPTH_TEST));
		glCall(glClear(GL_DEPTH_BUFFER_BIT));

		shadow_shader.Bind();

		for (const Model& obj : models)
		{
			obj.Draw(view, proj, shadow_shader, false);
		}
	}

}


