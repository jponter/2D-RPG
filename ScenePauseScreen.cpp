#include "ScenePauseScreen.hpp"


ScenePauseScreen::ScenePauseScreen(WorkingDirectory& workingDir,
	SceneStateMachine& sceneStateMachine,
	Window& window,
	ResourceAllocator<sf::Texture>& textureAllocator,
	ResourceAllocator<sf::Font>& fontAllocator,
	HeroClass& hero, Input& input)
	: sceneStateMachine(sceneStateMachine), workingDir(workingDir),
	window(window), switchToState(0), currentSeconds(0.f),
	showForSeconds(3.f), // We’ll show this splash screen for 3 seconds.
	textureAllocator(textureAllocator),fontAllocator(fontAllocator),
	hero(hero), input(input)
{}

void ScenePauseScreen::OnCreate()
{
	// We’ll initialise our Pause screen image here.



	int textureID = textureAllocator.Add(workingDir.Get()
		+ "baphomet.gif"); //1

	const int fontID = fontAllocator.Add(workingDir.Get() + "sansation.ttf");
	
	if (fontID >= 0)
	{
		std::shared_ptr<sf::Font> font = fontAllocator.Get(fontID);
		text.setFont(*font);
		text.setCharacterSize(64);
	}

	if (textureID >= 0) {



		std::shared_ptr<sf::Texture> texture = textureAllocator.Get(textureID);
		splashSprite.setTexture(*texture);

		sf::FloatRect spriteSize = splashSprite.getLocalBounds();
		splashSprite.setOrigin(spriteSize.width * 0.5f, spriteSize.height * 0.5f);
		splashSprite.setScale(0.5f, 0.5f);

		
	}

	text.setString("PAUSED - Press Space to Un-Pause");
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Magenta);
	text.setOutlineThickness(3.0f);
	
	
}

void ScenePauseScreen::OnActivate(unsigned int previousSceneID)
{
	// Resets the currentSeconds count whenever the scene is activated.
	SetSwitchToScene(previousSceneID);
	currentSeconds = 0.f;

	screenTexture = (sf::Texture)window.screenCopyTexture;
	screenSprite.setTexture(screenTexture, true);
	screenSprite.setColor(sf::Color(128, 128, 128));
	
	
}

void ScenePauseScreen::OnDestroy() { }

void ScenePauseScreen::SetSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
}

void ScenePauseScreen::Update(float deltaTime)
{
	//currentSeconds += deltaTime;
	

	if (window.HasFocus())
	{
		input.Update();

	}

	if (input.IsKeyUp(Input::Key::SPACE))
	{
		sceneStateMachine.SwitchTo(switchToState);
	}


	
}

void ScenePauseScreen::Draw(Window& window)
{
	sf::Vector2f windowCentre = window.GetView().getCenter();
	sf::Vector2f origin = window.mapPixelToCoords(sf::Vector2i(0, 0));
	splashSprite.setPosition(windowCentre.x, windowCentre.y);
	text.setPosition(windowCentre.x - 500 , windowCentre.y+200);
	screenSprite.setPosition(windowCentre.x - (window.GetView().getSize().x/2), windowCentre.y - (window.GetView().getSize().y/2));

	
	

	window.Draw(screenSprite);
	window.Draw(splashSprite);
	window.Draw(text);
}