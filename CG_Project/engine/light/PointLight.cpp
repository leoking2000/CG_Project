#include "PointLight.h"

namespace VE
{
	VE::PointLight::PointLight(const glm::vec3 pos)
		:
		position(pos),
		color(1.0f, 1.0f, 1.0f)
	{

	}

	void PointLight::Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader)
	{
		sphere.Bind();
		shader.Bind();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
		model = glm::scale(model, glm::vec3(size));

		shader.SetUniform("proj", proj);
		shader.SetUniform("view", view);
		shader.SetUniform("model", model);

		shader.SetUniform("color", color);

		glCall(glDrawElements(GL_TRIANGLES, sphere.indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

}


