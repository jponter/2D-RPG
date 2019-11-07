#pragma once
#ifndef SceneDungeon_hpp
#define SceneDungeon_hpp

#include "Scene.hpp"
#include "Input.hpp"
#include "WorkingDirectory.hpp"
#include "Object.hpp"
#include "C_Sprite.hpp"
#include "C_KeyboardMovement.hpp"
#include "ResourceAllocator.hpp"
#include "ObjectCollection.hpp"
#include "C_Animation.hpp"
#include "TileMapParser.hpp"
#include "C_BoxCollider.hpp"
#include "Debug.hpp"
#include "C_Camera.hpp"
#include "C_ProjectileAttack.hpp"
#include "C_Velocity.hpp"
#include "C_MovementAnimation.hpp"
#include "SharedContext.hpp"
#include "ImGuiLog.hpp"
#include "C_WarpLevelOnCollision.hpp"
#include "SceneStateMachine.hpp"
#include "Game.hpp"
#include "HeroClass.h"
#include "S_Command.hpp"
//#include "S_ScriptProcessor.hpp" moved to game class
#include "Raycast.hpp"
#include "C_InteractWithObjects.hpp"
#include "C_InteractableTalking.hpp"
#include "S_Quests.hpp"
#include "boost/assign/list_of.hpp"
#include <map>
#include "C_UIWorldLabel.hpp"
#include "C_EnemyHealth.hpp"
#include "BoxCast.hpp"
#include "C_AI.hpp"
#include "AI_Patrol.hpp"
#include "AI_Chase.hpp"

enum class npcTypes
{
	SKELETON,
	ORC,
	SIGNPOST,
	PLAYER
};



class SceneDungeon : public Scene
{
public:
	SceneDungeon(std::string LevelName, WorkingDirectory& workingDir,
		ResourceAllocator<sf::Texture>& textureAllocator, ResourceAllocator<sf::Font>& fontAllocator,
		Window& window, SceneStateMachine& stateMachine, ImGuiLog& mylog, HeroClass& hero, S_ScriptProcessor& scriptProcessor, list<S_Quests*>& listQuests, std::string level);

	//void ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser);
	void ChangeLevel1(std::string id, float posX, float posY);

	void OnCreate() override;
	void OnDestroy() override;

	void OnActivate(unsigned int previousSceneID) override;
	void OnDeactivate() override;

	void ProcessInput() override;
	void Update(float deltaTime) override;
	void Draw(Window& window) override;
	void LateUpdate(float deltaTime) override;

	 //std::shared_ptr<Object>& playerPtr;
	std::shared_ptr<Object> player;
	std::shared_ptr<Object> npc;
	
	

	//const ImGuiLog::ExampleAppLog& GetLog() const;

	void SetSwitchToScene(unsigned int id);

	DrawText Dialog;

	bool AddNpcToScene(std::string npcName, float x, float y, std::string npcType, bool persistant) override;
	bool inDialog() override;
	void SetDialog(bool value) override;

private:

	void CreatePlayer();
	void CreateFriend(std::string name, float x, float y);
	void ActivateTitle(Window* window, ResourceAllocator<sf::Font>* fontAllocator);

	

	void AddAnimationComponent(std::shared_ptr<Object> object, const int textureID, npcTypes EnemyType, bool additional);
	
	ResourceAllocator<sf::Texture>& textureAllocator;
	ResourceAllocator<sf::Font>& fontAllocator;

	WorkingDirectory& workingDir;
	Input input;
	SceneStateMachine& stateMachine;

	ObjectCollection objects;
	ObjectCollection dynamicObjects;
	TileMapParser mapParser;

	Window& window;

	bool m_exitDialog = false;
	
	SharedContext context;
	bool change = false;
	int switchto = 0;
	unsigned int switchToState;
	std::string nameLevel;
	std::string LevelName;
	ImGuiLog& mylog;
	HeroClass& hero;
	list<S_Quests*> &listQuests;

	//move to game glass - now a reference
	S_ScriptProcessor& m_script;
	
	std::string m_levelFile;
	float newPosX;
	float newPosY;

	S_Drawable drawableSystem;
	S_Drawable dynamicDrawableSystem;
	S_Collidable collisionSystem;
	Raycast raycast;
	BoxCast boxcast;
	
	Quadtree collisionTree;
	//thread th1;
	
};

#endif /* SceneDungeon_hpp */