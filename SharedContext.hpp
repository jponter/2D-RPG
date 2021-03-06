#pragma once
#ifndef SharedContext_hpp
#define SharedContext_hpp

#include "Input.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "Window.hpp"
#include "ImGuiLog.hpp"
#include "HeroClass.h"
#include <list>

//#include "Game.hpp"



class ObjectCollection;
class Scene;
class TileMapParser;
class Object;
class Quadtree;
class Raycast;
class BoxCast;
class S_ScriptProcessor;
class DrawText;
class S_Quests;
class Inventory;
class WorldDoors;

struct SharedContext
{
	Input* input;
	ObjectCollection* objects;
	ObjectCollection* dynamicObjects;
	WorkingDirectory* workingDir;
	ResourceAllocator<sf::Texture>* textureAllocator;
	ResourceAllocator<sf::Font>* fontAllocator;
	Window* window;
	ImGuiLog* imguilog;
	Scene* currentScene;
	//Scene* currentScene = nullptr;
	TileMapParser* mapParser;
	HeroClass* hero;
	std::shared_ptr<Object> player;
	Quadtree* collisionTree;
	Raycast* raycast;
	BoxCast* boxcast;
	S_ScriptProcessor* scriptEngine;
	DrawText* drawTextEngine;
	std::list<S_Quests*> *listQuests;
	std::string levelName;
	Inventory* playerInventory;
	WorldDoors* worldDoors;
	bool inDialog;
	
	//SceneStateMachine* sceneStateMachine;
	//Game* thisGame;
	
};

#endif /* SharedContext_hpp */

