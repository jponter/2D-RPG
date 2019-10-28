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
#include "S_ScriptProcessor.hpp"
#include "Raycast.hpp"
#include "C_InteractWithObjects.hpp"
#include "C_InteractableTalking.hpp"





class SceneDungeon : public Scene
{
public:
	SceneDungeon(WorkingDirectory& workingDir,
		ResourceAllocator<sf::Texture>& textureAllocator, ResourceAllocator<sf::Font>& fontAllocator,
		Window& window, SceneStateMachine& stateMachine, ImGuiLog& mylog, HeroClass& hero, std::string level);

	//void ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser);
	void ChangeLevel1(std::string id, float posX, float posY);

	void OnCreate() override;
	void OnDestroy() override;

	void OnActivate() override;
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

private:

	void CreatePlayer();
	void CreateFriend();

	

	void AddAnimationComponent(std::shared_ptr<Object> object, const int textureID);
	
	ResourceAllocator<sf::Texture>& textureAllocator;
	ResourceAllocator<sf::Font>& fontAllocator;

	WorkingDirectory& workingDir;
	Input input;
	SceneStateMachine& stateMachine;

	ObjectCollection objects;
	TileMapParser mapParser;

	Window& window;
	
	SharedContext context;
	bool change = false;
	int switchto = 0;
	unsigned int switchToState;
	std::string nameLevel;
	ImGuiLog& mylog;
	HeroClass& hero;

	S_ScriptProcessor m_script;
	
	std::string m_levelFile;
	float newPosX;
	float newPosY;

	S_Drawable drawableSystem;
	S_Collidable collisionSystem;
	Raycast raycast;
	
	Quadtree collisionTree;
	
};

#endif /* SceneDungeon_hpp */