#pragma once
#include "utilities/GL_Math.h"

namespace GL
{
	class GameObject
	{
	public:
		glm::mat4 transform;
		std::string model_name;
	public:
		GameObject(const std::string& model, const glm::mat4 mat)
			:
			model_name(model),
			transform(mat)
		{
		}
	};
}
