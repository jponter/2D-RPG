#include "SceneGame.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "SharedContext.hpp"
#include "C_Direction.hpp"




auto mylog = new ImGuiLog;


//constructor
SceneGame::SceneGame(WorkingDirectory& workingDir,
	ResourceAllocator<sf::Texture>& textureAllocator,
	Window& window)

	: workingDir(workingDir),
	textureAllocator(textureAllocator),
	mapParser(textureAllocator, context),
	window(window){}


void SceneGame::CreatePlayer()
{

	//std::shared_ptr<Object> player = std::make_shared<Object>(&context);
	player = std::make_shared<Object>(&context);

	//sf::Vector2f playerpos = player->transform->GetPosition();

	//playerPtr = player;
	player->transform->SetPosition(200, 700);

	// Adds a component by calling our previously written template function.
	auto sprite = player->AddComponent<C_Sprite>();
	//sprite->SetTextureAllocator(&textureAllocator);
	sprite->SetDrawLayer(DrawLayer::Entities);
	//sprite->Load(workingDir.Get() + "viking.png");
	//setting of sprite now handled by animation component



	//KEYBOARD MOVEMENT
	/*auto movement = player->AddComponent<C_KeyboardMovement>();
	movement->SetInput(&input);*/
	player->AddComponent<C_KeyboardMovement>();

	// add animation component
	

	int textureID = textureAllocator.Add(workingDir.Get()
		+ "Player.png");

	const unsigned int frameWidth = 64;
	const unsigned int frameHeight = 64;

	AddAnimationComponent(player, textureID);

	//end aminations


	//add collider component
	auto collider = player->AddComponent<C_BoxCollider>();
	collider->SetSize(frameWidth * 0.4f, frameHeight * 0.5f);
	collider->SetOffset(0.f, 14.f);
	collider->SetLayer(CollisionLayer::Player);

	//add the projectile
	//auto projectileAttack = player->AddComponent<C_ProjectileAttack>();
	/*projectileAttack->SetInput(&input);
	projectileAttack->SetObjectCollection(&objects);
	projectileAttack->SetWorkingDirectory(&workingDir);
	projectileAttack->SetTextureAllocator(&textureAllocator);*/
	player->AddComponent<C_ProjectileAttack>();



	//add the camera
	/*auto camera = player->AddComponent<C_Camera>();
	camera->SetWindow(&window);*/
	player->AddComponent<C_Camera>();


	//below line is an example of how to access components of player and call their functions
	//player->transform->SetPosition(100.0f, 10.0f);

	//add velocity component
	player->AddComponent<C_Velocity>();

	//add the movement component
	player->AddComponent<C_MovementAnimation>();

	//add the direction component
	player->AddComponent<C_Direction>();

	player->makePersistant();

	//add the player
	objects.Add(player);



}

void SceneGame::CreateFriend()
{
	npc = std::make_shared<Object>(&context);

	npc->transform->SetPosition(500, 900);

	auto sprite = npc->AddComponent<C_Sprite>();
	sprite->SetDrawLayer(DrawLayer::Entities);

	auto animation = npc->AddComponent<C_Animation>();

	const int textureID = textureAllocator.Add(workingDir.Get() + "Skeleton.png");

	const unsigned int frameWidth = 64;
	const unsigned int frameHeight = 64;

	AddAnimationComponent(npc, textureID);

	auto collider = npc->AddComponent<C_BoxCollider>();
	collider->SetSize(frameWidth * 0.4f, frameHeight * 0.5f);
	collider->SetOffset(0.f, 14.f);
	collider->SetLayer(CollisionLayer::NPC);

	/*auto warp1 = npc->AddComponent<C_WarpLevelOnCollision>();
	warp1->warplevel = 1;*/

	npc->AddComponent<C_Velocity>();
	npc->AddComponent<C_MovementAnimation>();
	npc->AddComponent<C_Direction>();
	npc->makePersistant();

	objects.Add(npc);



}

void SceneGame::AddAnimationComponent(std::shared_ptr<Object> object, const int textureID)
{

	auto animation = object->AddComponent<C_Animation>();

	const unsigned int frameWidth = 64;
	const unsigned int frameHeight = 64;

	const FacingDirection directions[4] = { FacingDirection::Up, FacingDirection::Left, FacingDirection::Down, FacingDirection::Right };

	/*******************
	 * Idle Animations *
	 *******************/
	const bool idleAnimationLooped = false;

	unsigned int idleYFramePos = 512;

	std::map<FacingDirection, std::shared_ptr<Animation>> idleAnimations;

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>();

		idleAnimation->AddFrame(textureID, 0, idleYFramePos, frameWidth, frameHeight, 0.f, idleAnimationLooped);

		idleAnimations.insert(std::make_pair(directions[i], idleAnimation));

		idleYFramePos += frameHeight;
	}

	animation->AddAnimation(AnimationState::Idle, idleAnimations);


	/**********************
	 * Walking Animations *
	 **********************/
	const bool walkAnimationLooped = true;
	const int walkingFrameCount = 9;
	const float delayBetweenWalkingFramesSecs = 0.1f;

	unsigned int walkingYFramePos = 512;

	std::map<FacingDirection, std::shared_ptr<Animation>> walkingAnimations;

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Animation> walkingAnimation = std::make_shared<Animation>();
		for (int i = 0; i < walkingFrameCount; i++)
		{
			walkingAnimation->AddFrame(textureID, i * frameWidth, walkingYFramePos, frameWidth, frameHeight, delayBetweenWalkingFramesSecs, walkAnimationLooped);
		}

		walkingAnimations.insert(std::make_pair(directions[i], walkingAnimation));

		walkingYFramePos += frameHeight;
	}

	animation->AddAnimation(AnimationState::Walk, walkingAnimations);


	/*************************
	 * Projectile Animations *
	 *************************/
	const bool projectileAnimationLooped = true;
	const int projectileFrameCount = 10;
	const float delayBetweenProjectileFramesSecs = 0.08f;

	std::map<FacingDirection, std::shared_ptr<Animation>> projectileAnimations;

	unsigned int projFrameYPos = 1024;

	for (int i = 0; i < 4; i++)
	{
		std::shared_ptr<Animation> projAnimation = std::make_shared<Animation>();
		for (int i = 0; i < projectileFrameCount; i++)
		{
			projAnimation->AddFrame(textureID, i * frameWidth, projFrameYPos, frameWidth, frameHeight, delayBetweenProjectileFramesSecs, projectileAnimationLooped);
		}
		projectileAnimations.insert(std::make_pair(directions[i], projAnimation));

		projFrameYPos += frameHeight;
	}

	animation->AddAnimation(AnimationState::Projectile, projectileAnimations);

}

void SceneGame::ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser)
{


	switch (level)
	{
	case 0:
	{
		
		objects.Clear();
		//textureAllocator.Clear();
		
		
		sf::Vector2i mapOffset(0, 0);

		std::vector<std::shared_ptr<Object>> levelTiles = mapParser.Parse(workingDir.Get() + "House Exterior New.tmx"
			, mapOffset);


		objects.Add(levelTiles);
		//create our player
		//CreatePlayer();
		//create our friend
		//CreateFriend();
		player->transform->SetPosition(320, 560);
		npc->transform->SetPosition(660, 700);
		objects.ProcessNewObjects();
		break;
	}
	case 1:
	{


		objects.Clear();
		//textureAllocator.Clear();
		sf::Vector2i mapOffset(-50, 128);

		std::vector<std::shared_ptr<Object>> levelTiles = mapParser.Parse(workingDir.Get() + "House Interior New.tmx"
			, mapOffset);


		objects.Add(levelTiles);
		//create our player
		//CreatePlayer();
		//create our friend
		//CreateFriend();
		player->transform->SetPosition(680, 1200);
		npc->transform->SetPosition(680, 800);
		objects.ProcessNewObjects();
		break;
	}
	
	}// end switch

	

	
	

}

void SceneGame::ChangeLevel1(int id)
{
	//ChangeLevel(id, objects, mapParser);
	change = true;
	switchto = id;
}

void SceneGame::OnCreate()
{
	//set the context
	context.input = &input;
	context.objects = &objects;
	context.workingDir = &workingDir;
	context.textureAllocator = &textureAllocator;
	context.window = &window;
	
	//update the context with mylog
	context.imguilog = mylog;
	context.currentScene = this;
	context.mapParser = &mapParser;
	

	

#ifdef _DEBUG
	//initialise IMGUI
	window.imGuiInit();
#endif
	
	//SceneGame::ChangeLevel(1, objects, mapParser);
	CreatePlayer();
	CreateFriend();


	ChangeLevel(0, objects, mapParser);
	
	
	//ChangeLevel(1, objects, mapParser);

	//Add the world
	//sf::Vector2i mapOffset(-100, 128);
	
	


}


//static void ShoweAppLog(bool* p_open)
//{
//	static ExampleAppLog log;
//
//	// For the demo: add a debug button _BEFORE_ the normal log window contents
//	// We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
//	// Most of the contents of the window will be added by the log.Draw() call.
//	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
//	ImGui::Begin("Example: Log", p_open);
//	if (ImGui::SmallButton("[Debug] Add 5 entries"))
//	{
//		static int counter = 0;
//		for (int n = 0; n < 5; n++)
//		{
//			const char* categories[3] = { "info", "warn", "error" };
//			const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
//			log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
//				ImGui::GetFrameCount(), categories[counter % IM_ARRAYSIZE(categories)], ImGui::GetTime(), words[counter % IM_ARRAYSIZE(words)]);
//			counter++;
//		}
//	}
//	ImGui::End();
//
//	// Actually call in the regular Log helper (which will Begin() into the same window as we just did)
//	log.Draw("Example: Log", p_open);
//}

void SceneGame::OnDestroy()
{

}

void SceneGame::ProcessInput()
{
	if (window.HasFocus())
	{
		input.Update();
	}
}

void SceneGame::Update(float deltaTime)
{
	if (change == true) {
		change = false;
		ChangeLevel(switchto, objects, mapParser);

	}
	
	objects.ProcessNewObjects();
	
	
	objects.Update(deltaTime);
	objects.ProcessRemovals();
	//check for the debug camera zoom
	Debug::HandleCameraZoom(window, input);
	

	//ImGui::ShowTestWindow();

	
	
}

void SceneGame::LateUpdate(float deltaTime)
{
	objects.LateUpdate(deltaTime);
}

//void SceneGame::SetSwitchToScene(unsigned int id)
//{
//	// Stores the id of the scene that we will transition to.
//	switchToState = id;
//}

void SceneGame::Draw(Window& window)
{
	objects.Draw(window);




#ifdef _DEBUG
	Debug::Draw(window);
	// put in the IMGUI stuff here - on future iterations may move this up the class list
	//window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;
	window.pollEvent();
	window.imGuiUpdate(deltaClock);

	
	//Debug::LogIM("test \n", mylog);

	mylog->mylog.Draw("Debug Log");
	
	
	ImGui::Begin("Window 1"); // begin window
	

	int numObjects = objects.size();

	sf::Vector2f playerpos = player->transform->GetPosition();
	
	
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
	

	ImGui::Text("Player X = %f, Y = %f", playerpos.x, playerpos.y);

	sf::Vector2u playerCoords;
	//playerCoords = playerTexture->getSize();
	

	//ImGui::Image(tmpsprite);
	//context.objects;
	
	// bool checkBox = false;
	//ImGui::Checkbox("Toggle", &checkBox);

	//ImGui::ShowDemoWindow();

	ImGui::End(); // end window


	window.imGuiRender();
	
	
	



#endif  // end _DEBUG
}