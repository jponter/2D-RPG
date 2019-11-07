#pragma once

#ifndef Scene_hpp
#define Scene_hpp

#include "Window.hpp"
#include "ObjectCollection.hpp"
#include "TileMapParser.hpp"


class Scene
{
public:
	// Called when scene initially created. Called once.
	virtual void OnCreate() = 0;

	// Called when scene destroyed. Called at most once (if a scene 
	// is not removed from the game, this will never be called).
	virtual void OnDestroy() = 0;

	// Called whenever a scene is transitioned into. Can be 
	// called many times in a typical game cycle.
	virtual void OnActivate(unsigned int previousSceneID) {};

	// Called whenever a transition out of a scene occurs. 
	// Can be called many times in a typical game cycle.
	virtual void OnDeactivate() {};

	// The below functions can be overridden as necessary in our scenes.
	virtual void ProcessInput() {};
	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};
	virtual void Draw(Window& window) {};
	virtual void ChangeLevel(int level, ObjectCollection& objects, TileMapParser& mapParser) {};
	virtual void ChangeLevel1(std::string id, float posX, float posY) {};
	virtual bool AddNpcToScene(std::string name, float x, float y, std::string npcType, bool persistant);
	virtual bool inDialog();
	virtual void SetDialog(bool value) {};
	
};


#endif /* Scene_hpp */