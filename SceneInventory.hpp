#pragma once
#ifndef SceneInventory_hpp
#define SceneInventory_hpp


#include "SceneStateMachine.hpp"
#include "WorkingDirectory.hpp"
#include "ResourceAllocator.hpp"
#include "SharedContext.hpp"
#include "Item.hpp"
#include "Inventory.hpp"

class SceneInventory :
	public Scene
{

	public:
	SceneInventory(WorkingDirectory& workingDir,
		SceneStateMachine& sceneStateMachine,
		Window& window,
		ResourceAllocator<sf::Texture>& textureAllocator,
		ResourceAllocator<sf::Font>& fontAllocator, HeroClass& hero, Input& input	);

	void SetContext(SharedContext& context) { m_context = &context; }
	void SetInventory(Inventory& inventory) { player_inventory = &inventory;  }

	void OnCreate() override;
	void OnDestroy() override;

	void OnActivate(unsigned int previousSceneID) override;

	void SetSwitchToScene(unsigned int id);

	void Update(float deltaTime) override;
	void Draw(Window& window) override;
	void InvalidateInventory();


private:
	
	
	SharedContext* m_context;
	Inventory* player_inventory;
	HeroClass& hero;
	WorkingDirectory& workingDir;
	SceneStateMachine& sceneStateMachine;
	Window& window;
	ResourceAllocator<sf::Texture>& textureAllocator;
	ResourceAllocator<sf::Font>& fontAllocator;

	Input& input;

	sf::Text text;
	sf::Text countText;
	sf::Text descriptionText;

	unsigned int switchToState;

	//std::vector<std::shared_ptr<Item>> items;
	std::vector<sf::Sprite> items_sprites;
	std::vector<std::unique_ptr<sf::RenderTexture>> items_textures;
	sf::View originalView;

	sf::Sprite BankStash;

	int column = 16;
	int row = 9*32;
	int yOff = 9 * 32;
	int xOff = 16;
	int scale = 2;
	sf::RectangleShape selectRect;
	int selectedRow;
	int selectedCol;
	int selectedInv;

};

#endif