#pragma once
#ifndef SceneStateMachine_hpp
#define SceneStateMachine_hpp

#include <memory>
#include <unordered_map>

#include "Scene.hpp"
#include "Window.hpp"
using namespace std;

class SceneStateMachine
{
public:
	SceneStateMachine();

	// ProcessInput, Update, LateUpdate, and Draw will simply be 
	// pass through methods. They will call the correspondingly 
	// named methods of the active scene.
	void ProcessInput();
	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	// Adds a scene to the state machine and returns the id of that scene.
	unsigned int Add(std::shared_ptr<Scene> scene);
	void AddSceneName(std::string name, int id);
	int GetSceneByName(std::string name);

	// Transitions to scene with specified id.
	void SwitchTo(unsigned int id);
	void SwitchTo(unsigned int id, unsigned int previous);

	// Removes scene from state machine.
	void Remove(unsigned int id);

	

private:
	// Stores all of the scenes associated with this state machine.
	std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;

	std::map<string, unsigned int> sceneMap;

	// Stores a reference to the current scene. Used when drawing/updating.
	std::shared_ptr<Scene> curScene;

	// Stores our current scene id. This is incremented whenever 
	// a scene is added.
	unsigned int insertedSceneID;

	// store the previous scene
	unsigned int previousSceneID;


	//should we change scene?
	bool changeScene = false;
};

#endif /* SceneStateMachine_hpp */