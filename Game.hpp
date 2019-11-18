#pragma once
#ifndef Game_hpp
#define Game_hpp

#include "Window.hpp"
#include "WorkingDirectory.hpp"
#include "Input.hpp"

//Scenes
#include "SceneStateMachine.hpp" // New
#include "SceneSplashScreen.hpp" // New
#include "ScenePauseScreen.hpp"
#include "SceneGame.hpp" // New
#include "SceneDungeon.hpp"
#include "SceneInventory.hpp"


#include "ResourceAllocator.hpp"
#include "ImGuiLog.hpp"
#include "imgui.h"
#include "SharedContext.hpp"
#include "HeroClass.h"
#include "DrawText.h"
#include "S_ScriptProcessor.hpp"
#include "S_Quests.hpp"
#include "Inventory.hpp"









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
	S_ScriptProcessor m_script;

	//Logger* logger;

private:
	Window window;
	WorkingDirectory workingDir;

	sf::Clock clock;
	float deltaTime;

	SceneStateMachine sceneStateMachine;
	ResourceAllocator<sf::Texture> textureAllocator;
	ResourceAllocator<sf::Font> fontAllocator;
	HeroClass hero;
	//ObjectCollection m_Inventory;

	Inventory m_Inventory;

	list<S_Quests*> m_listQuests;
	
	

	

	

	//store a lookup of the scene names with their ID's
	

	
	

};




#endif /* Game_hpp */