#include "SceneDungeon.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "SharedContext.hpp"
#include "C_Direction.hpp"




//auto mylog = new ImGuiLog;


//constructor
SceneDungeon::SceneDungeon(WorkingDirectory& workingDir,
	ResourceAllocator<sf::Texture>& textureAllocator,
	Window& window, SceneStateMachine& stateMachine, ImGuiLog& mylog)

	: workingDir(workingDir),
	textureAllocator(textureAllocator),
	mapParser(textureAllocator, context),
	window(window), stateMachine(stateMachine), mylog(mylog){}


void SceneDungeon::CreatePlayer()
{

	//std::shared_ptr<Object> player = std::make_shared<Object>(&context);
	player = std::make_shared<Object>(&context);

#ifdef _DEBUG
	//get the address of the player object and add it to the imgui debug
	std::stringstream buffer;
	buffer << &player << endl;
	std::string message = "Created a Player at 0x" + buffer.str();
	context.imguilog->mylog.AddLog(message.c_str());
#endif 

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

void SceneDungeon::CreateFriend()
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

void SceneDungeon::AddAnimationComponent(std::shared_ptr<Object> object, const int textureID)
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

//void SceneDungeon::ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser)
//{
//	std::vector<std::shared_ptr<Object>> levelTiles;
//
//	switch (level)
//	{
//
//		
//	case 0:
//	{
//		
//		objects.Clear();
//		//textureAllocator.Clear();
//		
//		
//		sf::Vector2i mapOffset(0, 0);
//
//		 levelTiles = mapParser.Parse(workingDir.Get() + "House Exterior New.tmx"
//			, mapOffset);
//
//
//		objects.Add(levelTiles);
//		//create our player
//		//CreatePlayer();
//		//create our friend
//		//CreateFriend();
//		player->transform->SetPosition(320, 440);
//		npc->transform->SetPosition(660, 700);
//		objects.ProcessNewObjects();
//		break;
//	}
//	case 1:
//	{
//
//		
//		objects.Clear();
//		//textureAllocator.Clear();
//		sf::Vector2i mapOffset(0, 0);
//
//		levelTiles = mapParser.Parse(workingDir.Get() + "House Interior New.tmx"
//			, mapOffset);
//
//
//		objects.Add(levelTiles);
//		//create our player
//		//CreatePlayer();
//		//create our friend
//		//CreateFriend();
//		player->transform->SetPosition(760, 1360);
//		npc->transform->SetPosition(760, 800);
//		objects.ProcessNewObjects();
//		break;
//	}
//	case 2: //dungeon
//	{
//		objects.Clear();
//		//textureAllocator.Clear();
//		sf::Vector2i mapOffset(0, 0);
//
//		levelTiles = mapParser.Parse(workingDir.Get() + "Dungeon.tmx"
//			, mapOffset);
//
//
//		objects.Add(levelTiles);
//		//create our player
//		//CreatePlayer();
//		//create our friend
//		//CreateFriend();
//		player->transform->SetPosition(100, 340);
//		npc->transform->SetPosition(280, 340);
//		objects.ProcessNewObjects();
//		break;
//	}
//	
//	}// end switch
//
//	
//
//	
//	
//
//}

void SceneDungeon::ChangeLevel1(std::string id)
{
	//ChangeLevel(id, objects, mapParser);
	change = true;
	nameLevel = id;
	//switchto = id;
}

void SceneDungeon::OnCreate()
{
	//set the context
	context.input = &input;
	context.objects = &objects;
	context.workingDir = &workingDir;
	context.textureAllocator = &textureAllocator;
	context.window = &window;
	context.imguilog = &mylog;
	
	//update the context with mylog
	//context.imguilog = mylog;
	context.currentScene = this;
	context.mapParser = &mapParser;
	
	sf::Vector2i mapOffset(0, 0);

	std::vector<std::shared_ptr<Object>> levelTiles;

	levelTiles = mapParser.Parse(workingDir.Get() + "Dungeon.tmx"
		, mapOffset);


	objects.Add(levelTiles);
	//create our player
	CreatePlayer();
	//create our friend
	//CreateFriend();

	player->transform->SetPosition(100, 340);
	//npc->transform->SetPosition(280, 340);
	objects.ProcessNewObjects();
	


	
	//SceneDungeon::ChangeLevel(1, objects, mapParser);
	//CreatePlayer();
	//CreateFriend();


	//ChangeLevel(2, objects, mapParser);
	
	
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

void SceneDungeon::OnDestroy()
{

}

void SceneDungeon::ProcessInput()
{
	if (window.HasFocus())
	{
		input.Update();
	}
}

void SceneDungeon::Update(float deltaTime)
{
	if (change == true) {
		change = false;
		//ChangeLevel(switchto, objects, mapParser);
		int id = 0;
		string name = nameLevel;
		id = stateMachine.GetSceneByName(name);
		if (id != -1)
		{
			stateMachine.SwitchTo(id);
		}
		else Debug::LogError("Level switch ID not found");
	}
	
	
	objects.ProcessNewObjects();
	
	
	objects.Update(deltaTime);
	objects.ProcessRemovals();
	//check for the debug camera zoom
	Debug::HandleCameraZoom(window, input);
	

	//ImGui::ShowTestWindow();

	
	
}

void SceneDungeon::LateUpdate(float deltaTime)
{
	objects.LateUpdate(deltaTime);
}

void SceneDungeon::SetSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
}

void SceneDungeon::Draw(Window& window)
{
	objects.Draw(window);
	
	



}