#include "Game.hpp"


Game::Game() : window("2D Game Engine")
{
	std::shared_ptr<SceneSplashScreen> splashScreen =
		std::make_shared<SceneSplashScreen>(workingDir,
			sceneStateMachine,
			window,textureAllocator); //1

	std::shared_ptr<SceneGame> gameScene =
		std::make_shared<SceneGame>(workingDir,textureAllocator,window);

	unsigned int splashScreenID = sceneStateMachine.Add(splashScreen); //2
	unsigned int gameSceneID = sceneStateMachine.Add(gameScene);

	splashScreen->SetSwitchToScene(gameSceneID); //3

	sceneStateMachine.SwitchTo(splashScreenID); //4

	deltaTime = clock.restart().asSeconds();

}


void Game::Update()
{
	window.Update();
	sceneStateMachine.Update(deltaTime);

}

void Game::LateUpdate()
{
	sceneStateMachine.LateUpdate(deltaTime);
}

void Game::Draw()
{
	window.BeginDraw();
	// add drawing code

	sceneStateMachine.Draw(window);

	// end drawing code
	window.EndDraw();

}

void Game::CalculateDeltaTime()
{
	deltaTime = clock.restart().asSeconds();
}

bool Game::IsRunning() const
{
	
	return window.IsOpen();
}

void Game::CaptureInput()
{
	sceneStateMachine.ProcessInput();
}