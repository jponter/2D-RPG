#pragma once
#ifndef SceneGame_hpp
#define SceneGame_hpp

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
#include "HeroClass.h"




class SceneGame : public Scene
{
public:
	SceneGame(WorkingDirectory& workingDir,
		ResourceAllocator<sf::Texture>& textureAllocator,
		Window& window, SceneStateMachine& stateMachine, ImGuiLog& mylog, HeroClass& hero);

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



	

private:

	void CreatePlayer();
	void CreateFriend();

	void SetSwitchToScene(unsigned int id);

	void AddAnimationComponent(std::shared_ptr<Object> object, const int textureID);
	
	ResourceAllocator<sf::Texture>& textureAllocator;

	WorkingDirectory& workingDir;
	Input input;

	ObjectCollection objects;
	TileMapParser mapParser;
	SceneStateMachine& stateMachine;

	Window& window;
	ImGuiLog& mylog;

	SharedContext context;
	bool change = false;
	int switchto = 0;
	//unsigned int switchToState;
	std::string nameLevel;
	HeroClass& hero;
	
};

#endif /* SceneGame_hpp */