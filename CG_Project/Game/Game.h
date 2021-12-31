#include "Engine.h"

class Game : public GL::Engine
{
public:
	Game();
	void Start();
	~Game();
private:
	glm::vec3 craft_pos = glm::vec3(-100.0f, 50.0f, 75.0f);

};

