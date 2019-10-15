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




class SceneDungeon : public Scene
{
public:
	SceneDungeon(WorkingDirectory& workingDir,
		ResourceAllocator<sf::Texture>& textureAllocator,
		Window& window, SceneStateMachine& stateMachine, ImGuiLog& mylog);

	//void ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser);
	void ChangeLevel1(std::string id);

	void OnCreate() override;
	void OnDestroy() override;

	void ProcessInput() override;
	void Update(float deltaTime) override;
	void Draw(Window& window) override;
	void LateUpdate(float deltaTime) override;

	 //std::shared_ptr<Object>& playerPtr;
	std::shared_ptr<Object> player;
	std::shared_ptr<Object> npc;
	
	

	//const ImGuiLog::ExampleAppLog& GetLog() const;

	void SetSwitchToScene(unsigned int id);

	

private:

	void CreatePlayer();
	void CreateFriend();

	

	void AddAnimationComponent(std::shared_ptr<Object> object, const int textureID);
	
	ResourceAllocator<sf::Texture>& textureAllocator;

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
	
};

#endif /* SceneDungeon_hpp */