#include "Game.h"

Game::Game()
	:
GL::Engine(1600, 900, "Code along Project", true)
{
	//
	// renderer settings
	//

	renderer.clear_color = glm::vec3(0.0f, 0.0f, 0.0f);

	//
	// Set up game
	//

}

void Game::Start()
{
	while (!win.ShouldWindowClose())
	{
		NewFrame();

		renderer.RenderText(win, 0, 0, "Code along!!!", 40);



		EndFrame();
	}
}

Game::~Game()
{
	GL::LogInfo("Destroyed Game");
}

