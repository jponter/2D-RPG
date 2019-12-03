#include "SceneInventory.hpp"



SceneInventory::SceneInventory(WorkingDirectory& workingDir,
	SceneStateMachine& sceneStateMachine,
	Window& window,
	ResourceAllocator<sf::Texture>& textureAllocator,
	ResourceAllocator<sf::Font>& fontAllocator,
	HeroClass& hero, Input& input)
	
	: sceneStateMachine(sceneStateMachine), workingDir(workingDir),
	window(window), textureAllocator(textureAllocator), fontAllocator(fontAllocator), hero(hero), input(input)
	{}

void SceneInventory::OnCreate()
{

	int textureID = textureAllocator.Add(workingDir.Get()
		+ "BankStash.png");

	std::shared_ptr<sf::Texture> texture = textureAllocator.Get(textureID);
	

	BankStash.setTexture(*texture);
	//BankStash.setScale(0.5,0.5);
	
	


	try
	{
		gui = window.getTgui();
		tgui::Theme theme{ "themes/Black.txt" };
		auto panel = tgui::Panel::create();
		panelptr = panel;
		panel->setRenderer(theme.getRenderer("Panel"));
		auto renderer = panel->getRenderer();
		renderer->setBackgroundColor(sf::Color::Transparent);


		auto textbox = tgui::TextBox::create();


		textbox->setRenderer(theme.getRenderer("TextBox"));
		textbox->setTextSize(20);
		textbox->setSize(400, 150);
		textbox->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
		textbox->setText("This is the textbox");

		textbox->setEnabled(false);
		descriptionBox = textbox;

		auto descriptionLabel = tgui::Label::create();
		descriptionLabel->setRenderer(theme.getRenderer("Label"));
		descriptionLabel->setText("Item Description:");
		descriptionLabel->setTextSize(30);
		descriptionLabel->setPosition({ bindLeft(textbox), bindTop(textbox) - (34) });

		sf::Vector2f pos = window.convertCoords(sf::Vector2i(635, 120), window.GetView());
		descriptionBox->setPosition(pos.x, pos.y);


		panel->add(textbox,"DescriptionTextBox");
		panel->add(descriptionLabel, "DescriptionLabel");
		panel->setVisible(false);
		gui->add(panel,"InventoryPanel");

	}

	catch (const tgui::Exception & e)
	{
		std::cerr << "TGUI Exception: " << e.what() << std::endl;
		//return EXIT_FAILURE;
	}
	
	
	



}

void SceneInventory::OnDestroy()
{
}

void SceneInventory::OnActivate(unsigned int previousSceneID)
{
	SetSwitchToScene(previousSceneID);
	originalView = window.GetView();


	//Turn off any shaders
	//todo: change this from window.isSnowing to window.shadersActivated or something
	shadersActivated = window.isSnowing;

	if (shadersActivated)
	{
		window.isSnowing = false;
	}

	const int fontID = fontAllocator.Add(workingDir.Get() + "Assets/Fonts/Charriot.ttf");

	if (fontID >= 0)
	{
		std::shared_ptr<sf::Font> font = fontAllocator.Get(fontID);

		countText.setFont(*font);
		countText.setCharacterSize(10);

		//descriptionText.setFont(*font);
		//descriptionText.setCharacterSize(20);
		gui = window.getTgui();
		gui->setFont(*font);
		
	}
	else(Debug::LogError("SceneInventory - Font not found - Charriot.ttf"));

	const int fontID2 = fontAllocator.Add(workingDir.Get() + "Assets/Fonts/Sansation.ttf");

	if (fontID2 >= 0)
	{
		std::shared_ptr<sf::Font> font2 = fontAllocator.Get(fontID2);
		text.setCharacterSize(64);
		text.setFont(*font2);
	}
	else(Debug::LogError("SceneInventory - Font not fount - Sansation.ttf"));

	text.setString("Inventory  - I to exit");
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Magenta);
	text.setOutlineThickness(3.0f);

	countText.setFillColor(sf::Color::White);
	countText.setOutlineColor(sf::Color::Black);
	countText.setOutlineThickness(1.0f);

	/*descriptionText.setFillColor(sf::Color::White);
	descriptionText.setOutlineColor(sf::Color::Black);
	descriptionText.setOutlineThickness(1.0f);

	descriptionText.setString("Item Description");*/

	//let's initialise the inventory screen




	int ScreenX = 16;
	int ScreenY = 9 * 32;
	int count = 0;

	sf::Vector2f origin = window.mapPixelToCoords(sf::Vector2i(0, 0));
	sf::Vector2f windowCentre = window.GetView().getCenter();
	sf::Vector2u windowSize = window.GetSize();
	sf::View view = window.GetView();

	items_sprites.clear();
	items_textures.clear();
	scale = 2;

	auto& items = player_inventory->GetInventory();
	for (auto& item : items)
	{
		ItemInfo iteminfo;
		item->GetInfo(iteminfo);
		Debug::Log("Have: " + iteminfo.name + " : " + iteminfo.data);
		sf::Sprite itemsprite;

		std::shared_ptr<sf::Texture> texture = textureAllocator.Get(iteminfo.textureId);
		itemsprite.setTexture(*texture);

		itemsprite.setTextureRect(sf::IntRect(iteminfo.column, iteminfo.row, 32, 32));




		std::unique_ptr<sf::RenderTexture> renderTex = std::make_unique<sf::RenderTexture>();
		renderTex->create(32, 32);
		renderTex->draw(itemsprite);

		countText.setString(sf::String(std::to_string(iteminfo.count)));
		countText.setPosition(sf::Vector2f(3, 0));
		renderTex->draw(countText);
		renderTex->display();

		items_textures.push_back(std::move(renderTex));

		itemsprite.setTexture(items_textures[count]->getTexture());
		itemsprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

		itemsprite.setScale(sf::Vector2f(scale, scale));
		sf::Vector2f pos = window.convertCoords(sf::Vector2i(ScreenX, ScreenY), view);
		itemsprite.setPosition(pos);

		ScreenX += 32 * scale;
		if (ScreenX > 8 * 32 * scale)
		{
			ScreenY += 32 * scale;
			ScreenX = 16;
		}

		items_sprites.push_back(itemsprite);

		count++;

	}

	BankStash.setPosition(window.convertCoords(sf::Vector2i(0, 0), window.GetView()));


	
	panelptr->setVisible(true);

}

void SceneInventory::OnDeactivate()
{
	panelptr->setVisible(false);

	if (shadersActivated)
	{
		window.isSnowing = true;
	}
}
	

void SceneInventory::SetSwitchToScene(unsigned int id)
{
	// Stores the id of the scene that we will transition to.
	switchToState = id;
}

void SceneInventory::Update(float deltaTime)
{
	auto& items = player_inventory->GetInventory();
	if (window.HasFocus())
	{
		input.Update();

	}

	if (input.IsKeyUp(Input::Key::I) || input.IsKeyUp(Input::Key::Esc))
	{
		window.SetView(originalView);
		sceneStateMachine.SwitchTo(switchToState);
	}


	if (input.IsKeyDown(Input::Key::Right))
	{
		if (column < (32*scale) *7 + (16))
		column += 32*scale;
	}

	if (input.IsKeyDown(Input::Key::Left))
	{
		if (column > 16  )
			column -= 32*scale;
	}

	if (input.IsKeyDown(Input::Key::Down))
	{
		if (row < ((32 * scale) * 7) + (9*32))
			row += 32 * scale;
	}

	if (input.IsKeyDown(Input::Key::Up))
	{
		if (row > 9*32)
			row -= 32 * scale;
	}


	//row = 9 * 32;
	selectedCol = (column - 16) / (32*scale);
	selectedRow = (row - 9*32) / (32*scale);
	selectedInv = (selectedRow * 8) + selectedCol;

	selectRect.setOutlineColor(sf::Color::Red);
	sf::View view = window.GetView();
		sf::Vector2f pos = window.convertCoords(sf::Vector2i(column, row), view);
		selectRect.setOutlineThickness(1);
		selectRect.setFillColor(sf::Color::Blue);
		selectRect.setSize(sf::Vector2f((32.0f * scale)-4, (32.0f * scale)-4));
		selectRect.setPosition(pos);

		if (selectedInv < items.size())
		{
			//descriptionText.setString(items[selectedInv]->GetDescription());
			descriptionBox->setText(items[selectedInv]->GetDescription());
			
		}
		else
		{
			//descriptionText.setString("");
			descriptionBox->setText("");
		}

		if (input.IsKeyUp(Input::Key::SPACE))
		{
			Debug::Log("Inventory selected row: " + std::to_string(selectedRow));
			Debug::Log("Inventory selected column: " + std::to_string(selectedCol));
			Debug::Log("Inventory position: " + std::to_string(selectedInv));

			if (selectedInv < items.size())
			{
				//we have an inventory
				Debug::Log("Inventory Item: " + items[selectedInv]->GetName());
				if (items[selectedInv]->OnUse(hero)) // returns true if we used the item
				{
				
					if (items[selectedInv]->GetInfo().count < 1)
					{
						//delete the item
						Debug::Log("REMOVE THE ITEM");
						items.erase(items.begin() + selectedInv);
						
						
					} // if this didnt fire we still have some of the item left

					//invalidate the inventory to update the screen
					InvalidateInventory();
				}
				else
				{
					// we didnt use the item
					Debug::Log("Item was not used");

				}
			}
			else
			{
				Debug::Log("No Item at this position");
			}
		}

}

void SceneInventory::InvalidateInventory()
{

	int ScreenX = 16;
	int ScreenY = 9 * 32;
	int count = 0;

	sf::Vector2f origin = window.mapPixelToCoords(sf::Vector2i(0, 0));
	sf::Vector2f windowCentre = window.GetView().getCenter();
	sf::Vector2u windowSize = window.GetSize();
	sf::View view = window.GetView();

	items_sprites.clear();
	items_textures.clear();
	scale = 2;

	auto& items = player_inventory->GetInventory();
	for (auto& item : items)
	{
		ItemInfo iteminfo;
		item->GetInfo(iteminfo);
		Debug::Log("Have: " + iteminfo.name + " : " + iteminfo.data);
		sf::Sprite itemsprite;

		std::shared_ptr<sf::Texture> texture = textureAllocator.Get(iteminfo.textureId);
		itemsprite.setTexture(*texture);

		itemsprite.setTextureRect(sf::IntRect(iteminfo.column, iteminfo.row, 32, 32));




		std::unique_ptr<sf::RenderTexture> renderTex = std::make_unique<sf::RenderTexture>();
		renderTex->create(32, 32);
		renderTex->draw(itemsprite);

		countText.setString(sf::String(std::to_string(iteminfo.count)));
		countText.setPosition(sf::Vector2f(3, 0));
		renderTex->draw(countText);
		renderTex->display();

		items_textures.push_back(std::move(renderTex));

		itemsprite.setTexture(items_textures[count]->getTexture());
		itemsprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

		itemsprite.setScale(sf::Vector2f(scale, scale));
		sf::Vector2f pos = window.convertCoords(sf::Vector2i(ScreenX, ScreenY), view);
		itemsprite.setPosition(pos);

		ScreenX += 32 * scale;
		if (ScreenX > 8 * 32 * scale)
		{
			ScreenY += 32 * scale;
			ScreenX = 16;
		}

		items_sprites.push_back(itemsprite);

		count++;

	}

	
	
	
}


void SceneInventory::Draw(Window& window)
{

	sf::Vector2f windowCentre = window.GetView().getCenter();
	
	text.setPosition(windowCentre.x - 500, windowCentre.y + 200);
	//descriptionText.setPosition(windowCentre.x , windowCentre.y - 400);

	


	


	window.Clear(sf::Color(sf::Color::Black));

	window.Draw(BankStash);

	window.Draw(selectRect);
	for (auto& sprite : items_sprites)
	{
		window.Draw(sprite);
		//countText.setString(sf::String(std::to_string(iteminfo.count)))
		
	}

	
	window.Draw(text);
	//window.Draw(descriptionText);
	
	


}

