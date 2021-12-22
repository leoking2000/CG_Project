#pragma once
#include "Mesh.h"
#include "Shader.h"

namespace VE
{
	class PointLight
	{
	public:
		PointLight(const glm::vec3 pos = { 0.0f, 0.0f, 0.0f });

		void Draw(const glm::mat4& view, const glm::mat4& proj, const Shader& shader);

	public:
		glm::vec3 position;
		glm::vec3 color;
		f32 quadratic_term = 0.005;
		f32 linear_term = 0.01;
	private:
		Mesh sphere{ Mesh::GenarateSphere(16) };
		static constexpr f32 size = 0.1f;
	};
}