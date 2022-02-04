#include "Engine.h"

class Game : public GL::Engine
{
public:
	Game();
	void Start();
	~Game();
private:
	void DebugUpdate();
	void Update();
	void MoveCraft();
	glm::vec2 Input();
private:
	glm::vec3 craft_pos{ -100.0f, 50.0f, 75.0f };
	glm::vec3 craft_facing{ 0.0f, 0.0f, -1.0f };
	glm::vec3 craft_right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 craft_up{ 0.0f, 1.0f, 0.0f };

	f32 speed = 80.0f;


	glm::vec4 rel_cam_pos{ 0.0f, 10.0f, 30.0f, 1.0f };

	GL::ObjLoader::Mesh collision_hull;
};

