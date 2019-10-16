#include "Game.hpp"

static auto mylog = new ImGuiLog;
auto hero = new HeroClass;


Game::Game() : window("2D Game Engine")
{
	std::shared_ptr<SceneSplashScreen> splashScreen =
		std::make_shared<SceneSplashScreen>(workingDir,
			sceneStateMachine,
			window,textureAllocator); //1

	std::shared_ptr<SceneGame> gameScene =
		std::make_shared<SceneGame>(workingDir,textureAllocator,window, sceneStateMachine, mylog, hero);

	std::shared_ptr<SceneDungeon> dungeonScene =
		std::make_shared<SceneDungeon>(workingDir, textureAllocator, window, sceneStateMachine, mylog, hero);




	unsigned int splashScreenID = sceneStateMachine.Add(splashScreen); //2
	unsigned int gameSceneID = sceneStateMachine.Add(gameScene);
	unsigned int dungeonSceneID = sceneStateMachine.Add(dungeonScene);
	
	sceneStateMachine.AddSceneName("gameScene", gameSceneID);
	sceneStateMachine.AddSceneName("dungeonScene", dungeonSceneID);

	splashScreen->SetSwitchToScene(dungeonSceneID); //3
	dungeonScene->SetSwitchToScene(gameSceneID);

	sceneStateMachine.SwitchTo(splashScreenID); //4

	deltaTime = clock.restart().asSeconds();

	window.imGuiInit();

}

ImGuiLog* GetLog()
{
	return mylog;
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

	sceneStateMachine.Draw(window);


#ifdef _DEBUG
	Debug::Draw(window);
	// put in the IMGUI stuff here - on future iterations may move this up the class list
	//window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;
	window.pollEvent();
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