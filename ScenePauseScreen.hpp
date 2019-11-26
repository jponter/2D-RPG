#pragma once
#ifndef ScenePauseScreen_hpp
#define ScenePaueScreen_hpp

#include "SceneStateMachine.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "HeroClass.h"



class ScenePauseScreen : public Scene
{



	
	public:
		ScenePauseScreen(WorkingDirectory& workingDir,
			SceneStateMachine& sceneStateMachine, Window& window,
			ResourceAllocator<sf::Texture>& textureAllocator, ResourceAllocator<sf::Font>& fontAllocator, HeroClass& hero, Input& input);

		void OnCreate() override;
		void OnDestroy() override;

		void OnActivate(unsigned int previousSceneID) override;

		void SetSwitchToScene(unsigned int id);

		void Update(float deltaTime) override;
		void Draw(Window& window) override;

	private:
		sf::Texture splashTexture;
		sf::Sprite splashSprite;
		sf::Sprite screenSprite;
		sf::Texture screenTexture;
		Input& input;

		WorkingDirectory& workingDir;
		SceneStateMachine& sceneStateMachine;
		Window& window;
		ResourceAllocator<sf::Texture>& textureAllocator;
		ResourceAllocator<sf::Font>& fontAllocator;
		HeroClass& hero;

		sf::Text text;

		// We want to show this scene for a set amount of time
		float showForSeconds;

		// How long the scene has currently been visible.
		float currentSeconds;

		// The state we want to transition to when this scenes time expires.
		unsigned int switchToState;
	


};

#endif

