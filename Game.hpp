#pragma once
#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "WorkingDirectory.hpp"
#include "Input.hpp"
#include "SceneStateMachine.hpp" // New
#include "SceneSplashScreen.hpp" // New
#include "SceneGame.hpp" // New
#include "SceneDungeon.hpp"
#include "ResourceAllocator.hpp"
#include "ImGuiLog.hpp"
#include "imgui.h"
#include "SharedContext.hpp"
#include "HeroClass.h"





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
	ImGuiLog* GetLog();

	//const ImGuiLog::ExampleAppLog& GetLog() const;
	ImGuiLog mylog;

private:
	Window window;
	WorkingDirectory workingDir;

	sf::Clock clock;
	float deltaTime;

	SceneStateMachine sceneStateMachine;
	ResourceAllocator<sf::Texture> textureAllocator;
	HeroClass hero;

	

	//store a lookup of the scene names with their ID's
	

	
	

};




#endif /* Game_hpp */