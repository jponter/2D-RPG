#include "Game.hpp"


//static auto mylog = new ImGuiLog;
//auto hero = new HeroClass;




Game::Game() : window("2D Game Engine") 
{
	//set some pointers

	S_Quests::g_script = &m_script;
	S_Quests::g_heroStats = &hero;

	//make the mouse cursor invisible
	window.SetMouseCursorVisible(false);
	
	std::shared_ptr<SceneSplashScreen> splashScreen =
		std::make_shared<SceneSplashScreen>(workingDir,
			sceneStateMachine,
			window,textureAllocator); //1

	std::shared_ptr<ScenePauseScreen> pauseScreen =
		std::make_shared<ScenePauseScreen>(workingDir,
			sceneStateMachine,
			window, textureAllocator, fontAllocator,
			hero);

	std::shared_ptr<SceneDungeon> gameScene =
		std::make_shared<SceneDungeon>("House Exterior",workingDir,textureAllocator,fontAllocator, window, sceneStateMachine, mylog, hero, m_script, m_listQuests, m_Inventory,  "House Exterior Animated.tmx");

	std::shared_ptr<SceneDungeon> dungeonScene =
		std::make_shared<SceneDungeon>("Creepy Dungeon",workingDir, textureAllocator, fontAllocator, window, sceneStateMachine, mylog, hero, m_script, m_listQuests, m_Inventory, "dungeon.tmx");

	// add a test quest
	//std::string QuestName = "Main Quest";
	m_listQuests.push_front(new S_Quests_MainQuest());


	unsigned int splashScreenID = sceneStateMachine.Add(splashScreen); //2
	unsigned int gameSceneID = sceneStateMachine.Add(gameScene);
	unsigned int dungeonSceneID = sceneStateMachine.Add(dungeonScene);
	unsigned int pauseSceneID = sceneStateMachine.Add(pauseScreen);
	
	sceneStateMachine.AddSceneName("House Exterior", gameSceneID);
	sceneStateMachine.AddSceneName("Creepy Dungeon", dungeonSceneID);
	sceneStateMachine.AddSceneName("pauseScreen", pauseSceneID);

	splashScreen->SetSwitchToScene(dungeonSceneID); //3
	dungeonScene->SetSwitchToScene(gameSceneID);

	sceneStateMachine.SwitchTo(splashScreenID); //4

	deltaTime = clock.restart().asSeconds();

	static auto mylog = new ImGuiLog;

	window.imGuiInit();


	
	

}

ImGuiLog* Game::GetLog()
{
	return &mylog;
	
}


void Game::Update()
{
	float fps = 1.0f / deltaTime;
	
	std::string title = "2D Game Engine - FPS: " + std::to_string(fps);
	window.SetTitle(title);
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
	sf::Clock deltaClock;
	window.pollEvent();


	sceneStateMachine.Draw(window);

	
	


#ifdef _DEBUG
	Debug::Draw(window);
	// put in the IMGUI stuff here - on future iterations may move this up the class list
	//window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	


	window.imGuiUpdate(deltaClock);


	//Debug::LogIM("test \n", mylog);

	mylog.mylog.Draw("Debug Log");
	
	


	ImGui::Begin("Window 1"); // begin window


	int numObjects = 0;//context.objects.size();

	//sf::Vector2f playerpos = player->transform->GetPosition();


	/*sf::Sprite tmpsprite;
	std::shared_ptr<sf::Texture> texture = textureAllocator.Get(3);
	tmpsprite.setTexture(*texture);*/

	/*
	for (auto& o : objects.get())
	{
		int instance = o->instanceID->Get();
		std::cout << instance << std::endl;
	}
	*/


	ImGui::Text("Player X = %f, Y = %f\n", hero.pos.x, hero.pos.y);
	ImGui::Text("Player Health = %i", hero.health);

	//sf::Vector2u playerCoords;
	//playerCoords = playerTexture->getSize();


	//ImGui::Image(tmpsprite);
	//context.objects;

	// bool checkBox = false;
	//ImGui::Checkbox("Toggle", &checkBox);

	//ImGui::ShowDemoWindow();

	ImGui::End(); // end window


	window.imGuiRender();






#endif  // end _DEBUG



	

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




