#pragma once
#ifndef SharedContext_hpp
#define SharedContext_hpp

#include "Input.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "Window.hpp"
#include "ImGuiLog.hpp"
#include "HeroClass.h"
//#include "Game.hpp"



class ObjectCollection;
class Scene;
class TileMapParser;

struct SharedContext
{
	Input* input;
	ObjectCollection* objects;
	WorkingDirectory* workingDir;
	ResourceAllocator<sf::Texture>* textureAllocator;
	Window* window;
	ImGuiLog* imguilog;
	Scene* currentScene;
	//Scene* currentScene = nullptr;
	TileMapParser* mapParser;
	HeroClass* hero;
	
	//SceneStateMachine* sceneStateMachine;
	//Game* thisGame;
	
};

#endif /* SharedContext_hpp */

