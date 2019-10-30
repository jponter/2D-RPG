#include "SceneDungeon.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include "SharedContext.hpp"
#include "C_Direction.hpp"

//MACROS
/*current commands
X(moveTo(object, float target x, float target y, float timetotake)
X(ShowDialog(vector strings) - eg: X(ShowDialog({"Hello!", "What are you doing here?"})
*/
#define X(n) m_script.AddCommand(new S_Command_ ## n) // shorthand for adding scripts to the script engine

enum npcTypes
{
	SKELETON,
	ORC,
	SIGNPOST
};


//auto mylog = new ImGuiLog;



//constructor
SceneDungeon::SceneDungeon(std::string LevelName, WorkingDirectory& workingDir,
	ResourceAllocator<sf::Texture>& textureAllocator, ResourceAllocator<sf::Font>& fontAllocator,
	Window& window, SceneStateMachine& stateMachine,
	ImGuiLog& mylog, HeroClass& hero, S_ScriptProcessor& scriptProcessor,
	list<S_Quests*>& listQuests,
	std::string level)

	: LevelName(LevelName), workingDir(workingDir),
	textureAllocator(textureAllocator),
	mapParser(textureAllocator, context),
	window(window), stateMachine(stateMachine), mylog(mylog), hero(hero), m_script(scriptProcessor), listQuests(listQuests),
	m_levelFile(level),	collisionSystem(collisionTree), objects(drawableSystem,collisionSystem), dynamicObjects(drawableSystem, collisionSystem), raycast(collisionTree), fontAllocator(fontAllocator){}


void SceneDungeon::ActivateTitle(Window* window, ResourceAllocator<sf::Font>* fontAllocator)
{
	sf::Text text;
	text.setString("TESTING");
	const int fontID = fontAllocator->Add(context.workingDir->Get() + "sansation.ttf");
	std::shared_ptr<sf::Font> font = fontAllocator->Get(fontID);
	text.setFont(*font);
	//sf::Vector2u screenSize = window.GetSize();
	int c = 255;
	text.setPosition(300, 400); // need this to be centered with local bounds etc will do that later!

	while (c > 0)
	{
		//window.BeginDraw();

		text.setFillColor(sf::Color(c, c, c));
		window->Draw(text);
		
		c = c - 1;
	}

}


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
	message = "Player ID " + std::to_string(player->instanceID->Get());
	context.imguilog->mylog.AddLog(message.c_str());
	context.imguilog->mylog.AddLog("\n");
#endif 

	//sf::Vector2f playerpos = player->transform->GetPosition();

	//playerPtr = player;
	//player->transform->SetPosition(200, 700);

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

	//cheating way to keep track of the player object
	context.player = player;
	player->transform->SetPosition(630, 815);
	player->tag->Set(Tag::Player);

	//add interaction with objects
	player->AddComponent<C_InteractWithObjects>();

	player->name = "James";

	//add the player
	objects.Add(player);



}

void SceneDungeon::CreateFriend(std::string name, float x, float y)
{
	npc = std::make_shared<Object>(&context);

	npc->transform->SetPosition(x, y);

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
	/*npc->AddComponent<C_InteractableTalking>();
	auto npcInteract = npc->GetComponent<C_InteractableTalking>();
	npcInteract->SetText(vector<std::string>{ "Skelly:", "Hello I Am Skelly!" });*/
	npc->makePersistant();
	npc->name = name;

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

void SceneDungeon::ChangeLevel1(std::string id, float posX, float posY)
{
	//ChangeLevel(id, objects, mapParser);
	change = true;
	nameLevel = id;
	newPosX = posX;
	newPosY = posY;
	//switchto = id;
}

void SceneDungeon::OnCreate()
{
	//set the context
	context.input = &input;
	context.objects = &objects;
	context.workingDir = &workingDir;
	context.textureAllocator = &textureAllocator;
	context.fontAllocator = &fontAllocator;
	context.window = &window;
	context.imguilog = &mylog;
	context.hero = &hero;
	context.collisionTree = &collisionTree;
	context.raycast = &raycast;
	context.scriptEngine = &m_script;
	context.drawTextEngine = &Dialog;
	context.listQuests = &listQuests;
	context.levelName = LevelName;
	
	//update the context with mylog
	//context.imguilog = mylog;
	context.currentScene = this;
	context.mapParser = &mapParser;
	
	sf::Vector2i mapOffset(0, 0);

	std::vector<std::shared_ptr<Object>> levelTiles;

	levelTiles = mapParser.Parse(workingDir.Get() + m_levelFile
		, mapOffset);

	//set the collision tree bounds
	sf::FloatRect newbounds = sf::FloatRect(0, 0, context.mapParser->WorldX, context.mapParser->WorldY);
	context.objects->collidables.SetQuadTreeBounds(newbounds);

	objects.Add(levelTiles);

	


	//create our player
	CreatePlayer();
	//context.player = &player;
	//create our friend
	//CreateFriend("Skelly", 500, 900);
	//CreateFriend("Jim", 600, 900);


	
	
	//npc->transform->SetPosition(280, 340);
	objects.ProcessNewObjects();
	dynamicObjects.ProcessNewObjects();
	
	

	
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

void SceneDungeon::OnDeactivate()
{
	//th1.join();
	//save the player position
	hero.pos = sf::Vector2f(newPosX, newPosY);
	
	dynamicObjects.Clear();

}

void SceneDungeon::OnActivate()
{
	if (hero.pos.x != 0) {
		player->transform->SetPosition(hero.pos);
	}

	//check if we need objects/npc's from the quest system
	for (auto q : listQuests)
	{
		q->PopulateDynamics(&dynamicObjects, this->LevelName, &context);
	}


	//this thread doesnt work - it is trying to access the window in a non safe way! 
	// left in so i can see how to start threads in future
	//std::thread t(&SceneDungeon::ActivateTitle, this, &window, &fontAllocator);
	
	std::shared_ptr<Object> levelLabelObject = std::make_shared<Object>(&context);
	auto levelLabel = levelLabelObject->AddComponent<C_UIWorldLabel>();
	levelLabel->SetDrawLayer(DrawLayer::UI);
	levelLabel->SetFontColour(sf::Color::White);
	levelLabel->SetText(LevelName);
	
	//levelLabelObject->transform->SetParent(player->transform);
	sf::Vector2u center = window.GetCentre();
	levelLabelObject->transform->SetPosition(center.x - 200,center.y);

	dynamicObjects.Add(levelLabelObject);
	
	

}

void SceneDungeon::ProcessInput()
{
	if (window.HasFocus())
	{
			input.Update();
		
	}

	if (input.IsKeyUp(Input::Key::T))
	{
		Debug::Log("T Key Pressed - executing script");
		//m_script.AddCommand(new S_Command_MoveTo(player, 600, 600, 5.0f));
		//X(ShowDialog({ "Hello!" }, &Dialog, &window));
		//X(ShowDialog({ "OOo. Another RPG with SFML!", "Are you insane?", "Thanks to Javidx9 for", "his videos on script engine!"}, &Dialog, &window));
		if (window.isSnowing == false)
		{
			window.isSnowing = true;
		}
		else if (window.isSnowing == true)
		{
			window.isSnowing = false;
		}


	}

	if (input.IsKeyUp(Input::Key::Esc))
	{
		window.close();
	}

	if (Dialog.m_bShowDialog)
	{
		player->userMovementEnabled = false;
		std::shared_ptr<C_Velocity> velocity = player->GetComponent<C_Velocity>();

		velocity->Set(0, 0);


		if (input.IsKeyUp(Input::Key::SPACE))
		{
			Dialog.m_bShowDialog = false;
			m_script.CompleteCommand();
			player->userMovementEnabled = true;
		}
	}
}

void SceneDungeon::Update(float deltaTime)
{
	//window.isSnowing = false;
	m_script.ProcessCommand(deltaTime);

	hero.pos = player->transform->GetPosition();

	
	
	objects.ProcessNewObjects();
	dynamicObjects.ProcessNewObjects();
	
	
	objects.Update(deltaTime);
	dynamicObjects.Update(deltaTime);

	objects.ProcessRemovals();
	dynamicObjects.ProcessRemovals();

	//check for the debug camera zoom
	Debug::HandleCameraZoom(window, input);
	
	window.fElapsedtime += deltaTime;
	//ImGui::ShowTestWindow();

	if (change == true) {
		change = false;
		//ChangeLevel(switchto, objects, mapParser);
		int id = 0;
		string name = nameLevel;
		id = stateMachine.GetSceneByName(name);
		if (id != -1)
		{
			//player->transform->SetPosition(320, 440);
			stateMachine.SwitchTo(id);

			//player->transform->SetPosition(newPosX, newPosY);
		}
		else Debug::LogError("Level switch ID not found");
	}

	
	
}

void SceneDungeon::LateUpdate(float deltaTime)
{
	objects.LateUpdate(deltaTime);
	dynamicObjects.LateUpdate(deltaTime);
}



void SceneDungeon::SetSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
}

bool SceneDungeon::AddNpcToScene(std::string npcName, float x, float y, std::string npcType, bool persistant)
{

	std::shared_ptr<Object> npc = std::make_shared<Object>(&context);
	const unsigned int tileScale = 2;

	// Calculate world position.
	//objects are in world space - not in tile offsets - so we need to convert
	//float x = ((objX)/32) * 32* tileScale + offset.x;
	//float y = ((objY)/32) * 32* tileScale + offset.y;



	npc->transform->SetPosition(x, y);

	auto sprite = npc->AddComponent<C_Sprite>();
	sprite->SetDrawLayer(DrawLayer::Entities);

	auto animation = npc->AddComponent<C_Animation>();


	//type of NPC - 
	int textureID = 0;
	std::map<std::string, npcTypes> npcMap = boost::assign::map_list_of("Skeleton", SKELETON)("Orc", ORC)("Signpost",SIGNPOST);
	switch (npcMap[npcType])
	{
	case SKELETON:
		textureID = textureAllocator.Add(context.workingDir->Get() + "Skeleton.png");
		break;
	case ORC:
		textureID = textureAllocator.Add(context.workingDir->Get() + "Orc.png");
		break;
	}// end switch




	const unsigned int frameWidth = 64;
	const unsigned int frameHeight = 64;
	if (textureID != 0)
	{
		AddAnimationComponent(npc, textureID);
	}

	auto collider = npc->AddComponent<C_BoxCollider>();
	collider->SetSize(frameWidth * 0.4f, frameHeight * 0.5f);
	collider->SetOffset(0.f, 14.f);
	collider->SetLayer(CollisionLayer::NPC);

	/*auto warp1 = npc->AddComponent<C_WarpLevelOnCollision>();
	warp1->warplevel = 1;*/

	npc->AddComponent<C_Velocity>();
	npc->AddComponent<C_MovementAnimation>();
	npc->AddComponent<C_Direction>();
	npc->AddComponent<C_InteractableTalking>();
	auto npcInteract = npc->GetComponent<C_InteractableTalking>();
	if (npcType == "Signpost")
	{
		npcInteract->SetText(vector<std::string>{"[" + npcName + "]" });
	}
	else
	{
		npcInteract->SetText(vector<std::string>{"[" + npcName + "]", "Hello, I Am " + npcName + "!", "I have nothing for you right now." });
	}
	npc->name = npcName;

	if (!persistant)
	{
		dynamicObjects.Add(npc);
	}
	else
	{
		objects.Add(npc);
	}



	return true;
}

void SceneDungeon::Draw(Window& window)
{
	//we really want to do a quad tree search here for objects within our view
	/*sf::FloatRect rect = context.window->GetViewSpace();
	
	std::vector<std::shared_ptr<Object>>  o = objects.worldTree.Search(rect);*/

	//edit do it in the drawables collection - 

	objects.Draw(window);
	dynamicObjects.Draw(window);

	
	if (Dialog.m_bShowDialog)
	{
		sf::Vector2f PlayerCoord = player->transform->GetPosition();

		if ((PlayerCoord.y - 200) < 0) PlayerCoord.y = 420;
		if ((PlayerCoord.x + 100) > window.GetSize().x) PlayerCoord.x -= 150;
		Dialog.displayDialog(Dialog.m_vecDialogToShow, window, PlayerCoord.x -200, PlayerCoord.y - 200,  fontAllocator, context);
		
	}

	
}

