#pragma once
#include "VE_Math.h"
#include "Window.h"

namespace VE
{
	class Camera
	{
	public:
		glm::vec3 pos;
		glm::vec3 dir;

		float yaw = -PI / 2.0f;
		float pitch = 0.0f;

		bool mouseInput = false;
		bool x_press = false;
	public:
		Camera();

		Camera(const glm::vec3& in_pos, const glm::vec3& in_dir);

		glm::mat4 GetCameraView();

		void Update(Window& win, float dt);
	};
}