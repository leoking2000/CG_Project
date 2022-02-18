#include "Engine.h"
#include "ScorePoint.h"

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
	void OnCollision_ScorePoint();
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
	const glm::vec4 rel_cam_pos{ 0.0f, 10.0f, 30.0f, 1.0f };
	const f32 max_y = 80.0f;
private:
	// ScorePoints
	std::vector<ScorePoint> score_points;
	u32 score;

private:
	GL::ObjLoader::Mesh collision_hull;
};

