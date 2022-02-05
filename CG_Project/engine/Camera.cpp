#include "Camera.h"

namespace GL
{
	Camera::Camera()
		:
		pos(0.0f, 0.0f, 0.0f),
		dir(0.0f, 0.0f, -1.0f)
	{

	}

	Camera::Camera(const glm::vec3& in_pos, const glm::vec3& in_dir)
		:
		pos(in_pos),
		dir(glm::normalize(in_dir))
	{

	}

	glm::mat4 Camera::GetCameraView()
	{
		return glm::lookAt(pos, pos + dir, up);
	}

	void Camera::Update(Window& win, float dt)
	{
		float move_speed = 10.0f;

		if (win.KeyIsPress(GLFW_KEY_W))
		{
			pos = pos + dir * move_speed * dt;
		}
		if (win.KeyIsPress(GLFW_KEY_S))
		{
			pos = pos - dir * move_speed * dt;
		}

		if (win.KeyIsPress(GLFW_KEY_D))
		{
			pos += glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed * dt;
		}
		if (win.KeyIsPress(GLFW_KEY_A))
		{
			pos -= glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * move_speed * dt;
		}

		glm::vec2 mousePos = win.MousePos();

		static bool firstMouse = true;
		static double lastX;
		static double lastY;

		if (firstMouse)
		{
			lastX = mousePos.x;
			lastY = mousePos.y;
			firstMouse = false;
		}

		double xoffset = mousePos.x - lastX;
		double yoffset = lastY - mousePos.y;
		lastX = mousePos.x;
		lastY = mousePos.y;

		double sensitivity = 0.05 * dt;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		if (win.KeyIsPress(GLFW_KEY_X))
		{
			if (x_press == false)
			{
				mouseInput = !mouseInput;
				x_press = true;
			}
		}
		else
		{
			x_press = false;
		}

		if (mouseInput == false)
		{
			win.SetMouseVisibility(true);
		}
		else
		{
			win.SetMouseVisibility(false);

			yaw += (float)xoffset;
			pitch += (float)yoffset;

			if (pitch > PI / 2.0f)
				pitch = PI / 2.0f;
			if (pitch < -PI / 2.0f)
				pitch = -PI / 2.0f;

			glm::vec3 direction;
			direction.x = glm::cos(yaw) * glm::cos(pitch);
			direction.y = glm::sin(pitch);
			direction.z = glm::sin(yaw) * glm::cos(pitch);
			dir = glm::normalize(direction);
		}
	}
}


