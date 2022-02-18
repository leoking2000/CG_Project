#pragma once
#include "GameObject.h"
#include <vector>
#include <functional>

class ScorePoint
{
public:
	ScorePoint(glm::vec3 pos, std::string model, std::vector<GL::GameObject>& objects);

	void Tick(const glm::vec3& craft_pos, f32 dt, std::function<void()> OnCollition, std::vector<GL::GameObject>& objects);
private:
	// the index that can be used to get the game object from
	// the objects vector
	u32 m_index;

	// store the ScorePoint's world position
	glm::vec3 m_pos;

	// store the model of the game object in order to
	// set the game object's name to empty 
	// when the ScorePoint is deactivated
	std::string m_model;

	static constexpr f32 size = 4.0f;
private:
	// timer
	f32 max_inactive_time = 1000.0f; // in ms
	f32 time_inactive = 0;

};

