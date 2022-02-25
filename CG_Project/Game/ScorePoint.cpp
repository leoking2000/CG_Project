#include "ScorePoint.h"

ScorePoint::ScorePoint(glm::vec3 pos, std::string model, std::vector<GL::GameObject>& objects)
{
	m_index = (u32)objects.size();
	m_pos = pos;
	m_model = std::move(model);

	objects.emplace_back(m_model, glm::translate(pos) * glm::scale(glm::vec3(size)));
}

void ScorePoint::Tick(const glm::vec3& craft_pos, f32 dt, std::function<void()> OnCollition, std::vector<GL::GameObject>& objects)
{
	GL::GameObject& go = objects[m_index]; // get the gameobject

	// if ScorePoint is inactive
	if (go.model_name.empty())
	{
		time_inactive += dt;

		if (time_inactive > max_inactive_time)
		{
			time_inactive = 0;

			// activate ScorePoint
			go.model_name = m_model;
		}
	}
	else // if ScorePoint is active
	{
		if (glm::distance(craft_pos, m_pos) <= 1.5f * size)
		{
			// deactivate object
			go.model_name = "";
			OnCollition();
		}
	}
}
