#pragma once
#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "WorkingDirectory.hpp"
#include "Input.hpp"
#include "SceneStateMachine.hpp" // New
#include "SceneSplashScreen.hpp" // New
#include "SceneGame.hpp" // New
#include "ResourceAllocator.hpp"





class Game
{
public:
	Game();

	//void ProcessInput();
	void Update();
	void LateUpdate();
	void Draw();
	bool IsRunning() const;
	void CalculateDeltaTime();
	void CaptureInput();

private:
	Window window;
	WorkingDirectory workingDir;

	sf::Clock clock;
	float deltaTime;

	SceneStateMachine sceneStateMachine;
	ResourceAllocator<sf::Texture> textureAllocator;

	
	

};




#endif /* Game_hpp */