#include "Engine.h"

class Game : public GL::Engine
{
public:
	Game();
	~Game();
	void Start();
private:
	void Reset();
	void DebugUpdate();
	void GameUpdate();
private:
	void MoveCraft();
	bool IsCollidingWithSpheres();
	void OnCollisionWithSpheres();
	glm::vec2 Input();
private:
	enum GameState
	{
		START,
		PLAY,
		GAMEOVER
	};

	GameState state;
private:
	// craft variables
	glm::vec3 craft_pos;
	glm::vec3 craft_facing;
	glm::vec3 craft_right;
	glm::vec3 craft_up;
	f32 craft_speed;
	glm::vec4 rel_cam_pos{ 0.0f, 10.0f, 30.0f, 1.0f };
private:
	// spheres variables
	f32 sphere_size = 3.0f;
	std::vector<glm::vec3> spheres_locations;
	u32 score;
	bool has_collide_last_frame;
private:
	GL::ObjLoader::Mesh collision_hull;
};

