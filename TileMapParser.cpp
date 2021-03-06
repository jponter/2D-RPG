#include "TileMapParser.hpp"
#include "Utilities.hpp"
#include "C_WarpLevelOnCollision.hpp"
#include <iostream>
#include "WorkingDirectory.hpp"
#include "C_Animation.hpp"
#include <string>
#include "SceneDungeon.hpp"
#include "boost/assign/list_of.hpp"
#include <map>

//enum npcTypes
//{
//	SKELETON,
//	ORC
//};

TileMapParser::TileMapParser(ResourceAllocator<sf::Texture>& textureAllocator, SharedContext& context)
	: textureAllocator(textureAllocator),
	context(context)
			
{}

std::vector<std::shared_ptr<Object>>
TileMapParser::Parse(const std::string& file, sf::Vector2i offset)
{
	char* fileLoc = new char[file.size() + 1]; // 1

	//TODO: make multi format version of string copy
#ifdef MACOS
	strlcpy(fileLoc, file.c_str(), file.size() + 1);
#else
	strcpy_s(fileLoc, file.size() + 1, file.c_str());
#endif 

	//TODO: error checking - check file exists before attempting open.
	rapidxml::file<> xmlFile(fileLoc);
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* rootNode = doc.first_node("map");

		// Loads tile layers from XML.
		std::shared_ptr<MapTiles> map = BuildMapTiles(rootNode);

	// We need these to calculate the tiles position in world space
	int tileSizeX = std::atoi(rootNode->first_attribute("tilewidth")->value());
	int tileSizeY = std::atoi(rootNode->first_attribute("tileheight")->value());
	int mapsizeX = std::atoi(rootNode->first_attribute("width")->value());
	int mapsizeY = std::atoi(rootNode->first_attribute("height")->value());
	const unsigned int tileScale = 2;
	WorldX = mapsizeX * tileSizeX * tileScale;
	WorldY = mapsizeY * tileSizeY * tileScale;

	// This will contain all of our tiles as objects.
	std::vector<std::shared_ptr<Object>> tileObjects;
	int layerCount = map->size() - 1;

	// 2
	// We iterate through each layer in the tile map
	for (const auto& layer : *map)
	{
		// And each tile in the layer
		for (const auto& tile : layer.second->tiles)
		{
			std::shared_ptr<TileInfo> tileInfo = tile->properties;

			std::shared_ptr<Object> tileObject = std::make_shared<Object>(&context);

			const unsigned int tileScale = 2;
			animatedTileFound = false;

		

			//TODO: tile scale should be set at the data level.
			
			if (layer.second->isVisible)
			{
				// Allocate sprite.
				auto sprite = tileObject->AddComponent<C_Sprite>();
				//sprite->SetTextureAllocator(&textureAllocator);
				sprite->Load(tileInfo->textureID);
				sprite->SetTextureRect(tileInfo->textureRect);
				sprite->SetScale(tileScale, tileScale);
				//set the sort order for the tile based on layer
				sprite->SetSortOrder(layerCount);
				
				// we want to set the draw layer based on the Tiled Map - for now if there is any layer label that include "Front of Player" we add that to the new InFront Layer - which is drawn last!

				std::size_t found = layer.first.find("Front of Player");
				if (found == std::string::npos)
				{
					sprite->SetDrawLayer(DrawLayer::Background);
				}
				else
				{
					sprite->SetDrawLayer(DrawLayer::InFront);
				}
			}
			// Calculate world position.
			float x = tile->x * tileSizeX * tileScale + offset.x;
			float y = tile->y * tileSizeY * tileScale + offset.y;
			tileObject->transform->SetPosition(x, y);

			// We only add colliders to the one layer.
			if (layer.first == "Collisions")
			{
				auto collider = tileObject->AddComponent<C_BoxCollider>();
				float left = x - (tileSizeX * tileScale) * 0.5f;
				float top = y - (tileSizeY * tileScale) * 0.5f;
				float width = tileSizeX * tileScale;
				float height = tileSizeY * tileScale;
				collider->SetCollidable(sf::FloatRect(left, top, width, height));
				collider->SetLayer(CollisionLayer::Tile);
			}
			
			// if the tile has an animation set add them now
			int tileID = tileInfo->tileID;
			//std::cout << tileID << std::endl;
			AnimationIterator = AnimationIndex.find(tileID);
			if (AnimationIterator != AnimationIndex.end())
			{
				animatedTileFound = true;
				int foundTile = AnimationIterator->first;
				std::cout << "While Building tile objects - found a tile with animation ID: " << foundTile << std::endl;
				// what are the frames for this animation :| ???
				//we need a direction & velocity at the moment
				tileObject->AddComponent<C_Direction>();
				tileObject->AddComponent<C_Velocity>();


				//add the animation to the warp tile
				auto animation = tileObject->AddComponent<C_Animation>();

				const unsigned int frameWidth = 32;
				const unsigned int frameHeight = 32;

				const bool idleAnimationLooped = true;
				
				std::map<FacingDirection, std::shared_ptr<Animation>> idleAnimations;
				const int idleFrameCount = 64;

				std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>();
				
				for (auto frame : AnimationIterator->second)
				{
					// we should have animation frame rects
					float delayBetweenFrames = (1.0f / 1000.0f) * frame.duration;
					std::cout << "Rect: " << frame.x << "  " << frame.y << std::endl;
					idleAnimation->AddFrame(tileInfo->textureID, frame.x, frame.y, frameWidth, frameHeight, delayBetweenFrames, idleAnimationLooped);
					
				}
				idleAnimations.insert(std::make_pair(FacingDirection::Down, idleAnimation));

				animation->AddAnimation(AnimationState::Idle, idleAnimations);

				


			}


			if (animatedTileFound)
			{
				//animated tile - put it in dynamic objects - this is a bit hacky, but we know the dynamic objects collection exists;
				tileObject->makePersistant();
				context.dynamicObjects->Add(tileObject);
			}
			else
			{
				// Add new tile Object to the collection.
				tileObjects.emplace_back(tileObject);
			}
		}
		//decrement the layer count
		layerCount--;
	}

	//here i want to go through the objects layer and extract warp points
	int objX;
	int objY;
	int objWidth = 32;
	int objHeight = 32;
	std::string objToLevel = "";
	float toX = 0;
	float toY = 0;
	xml_node<> *node = doc.first_node("map");
	node = node->first_node("objectgroup");
	//node = node->first_node("object");
	
	if (node  != NULL)
	{
		for (node = node->first_node("object"); node; node = node->next_sibling())
		{
			//copy back the node
			xml_node<>* nodecopy = node;
			//node = node->first_node("object");
			std::cout << "Node Object Group has value " << node->name() << "\n";

			//Warp Points from the MAP
			if (std::string(node->first_attribute("name")->value()) == "Warp Point")
			{
				std::cout << "Warp Point found!" << std::endl;
				objX = std::atoi(node->first_attribute("x")->value());
				objY = std::atoi(node->first_attribute("y")->value());
				objWidth = 32;
				objHeight = 32;


				std::cout << "X = " << objX << std::endl;
				std::cout << "Y = " << objY << std::endl;
				std::cout << "Height = " << objHeight << std::endl;
				std::cout << "Width = " << objWidth << std::endl;



				for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
				{
					std::cout << "node Object Group has attribute " << attr->name() << " ";
					std::cout << "with value " << attr->value() << "\n";

				}
				node = node->first_node("properties");
				node = node->first_node("property");
				std::cout << "Node Property has value " << node->value() << "\n";
				std::string toLevel = node->first_attribute("value")->value();
				std::cout << "Warp to Level = " << toLevel << std::endl;

				node = node->next_sibling("property");
				toX = std::atof(node->first_attribute("value")->value());
				node = node->next_sibling("property");
				toY = std::atof(node->first_attribute("value")->value());

				objToLevel = toLevel;

				/*for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
				{
					std::cout << "node Property has attribute " << attr->name() << " ";
					std::cout << "with value " << attr->value() << "\n";

				}*/

				node = nodecopy;

				//todo: move the object addition code to the scene class as we did below for NPC's
				//add the tile object
				std::shared_ptr<Object> tileObject = std::make_shared<Object>(&context);
				const unsigned int tileScale = 2;

				// Calculate world position.
				//objects are in world space - not in tile offsets - so we need to convert
				//float x = ((objX)/32) * 32* tileScale + offset.x;
				//float y = ((objY)/32) * 32* tileScale + offset.y;

				float x = objX * tileScale + offset.x;
				float y = objY * tileScale + offset.y;

				tileObject->transform->SetPosition(x, y);


				// now this is a vortex always
				const int textureID = textureAllocator.Add(context.workingDir->Get() + "vortexanim256.png");
				auto sprite = tileObject->AddComponent<C_Sprite>();
				//sprite->SetTextureAllocator(&textureAllocator);
				//sprite->Load(textureID);
				//sprite->SetTextureRect(tileInfo->textureRect);
				sprite->SetScale(tileScale, tileScale);
				//set the sort order for the tile based on layer
				sprite->SetSortOrder(layerCount);
				// Set the tiles layer to background for now
				sprite->SetDrawLayer(DrawLayer::Entities);

				//we need a direction & velocity at the moment
				tileObject->AddComponent<C_Direction>();
				tileObject->AddComponent<C_Velocity>();


				//add the animation to the warp tile
				auto animation = tileObject->AddComponent<C_Animation>();

				const unsigned int frameWidth = 32;
				const unsigned int frameHeight = 32;

				const bool idleAnimationLooped = true;
				const float delayBetweenFrames = 0.1f;

				// sprite tilesheet is 8x8

				std::map<FacingDirection, std::shared_ptr<Animation>> idleAnimations;
				const int idleFrameCount = 64;

				std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>();
				for (int y = 0; y < 8; y++)
				{
					for (int x = 0; x < 8; x++)
					{
						idleAnimation->AddFrame(textureID, x * frameWidth, y * frameHeight, frameWidth, frameHeight, delayBetweenFrames, idleAnimationLooped);
					}
				}
				idleAnimations.insert(std::make_pair(FacingDirection::Down, idleAnimation));

				animation->AddAnimation(AnimationState::Idle, idleAnimations);


				auto collider = tileObject->AddComponent<C_BoxCollider>();
				//float left = (x - (tileSizeX * tileScale)) - (tileSizeX* tileScale);
				//float top = y - (tileSizeY * tileScale);
				float width = tileSizeX * tileScale;
				float height = tileSizeY * tileScale;
				float left = x - (tileSizeX * tileScale) * 0.5f;
				float top = y - (tileSizeY * tileScale) * 0.5f;
				collider->SetCollidable(sf::FloatRect(left, top, width, height));
				collider->SetLayer(CollisionLayer::Tile);
				auto warp1 = tileObject->AddComponent<C_WarpLevelOnCollision>();
				warp1->warplevel = objToLevel;
				warp1->toX = toX;
				warp1->toY = toY;
				tileObject->makePersistant(); // we dont want these to dissapear on a level change!



				// Add new tile Object to the collection.
				//tileObjects.emplace_back(tileObject);
				context.dynamicObjects->Add(tileObject);

			} //end warp


			//Items from the MAP!

			if (std::string(node->first_attribute("name")->value()) == "ITEM")
			{

				//add an item from the map
				std::cout << "Item Found!" << std::endl;
				objX = std::atoi(node->first_attribute("x")->value());
				objY = std::atoi(node->first_attribute("y")->value());
				objWidth = 32;
				objHeight = 32;

				std::cout << "X = " << objX << std::endl;
				std::cout << "Y = " << objY << std::endl;
				std::cout << "Height = " << objHeight << std::endl;
				std::cout << "Width = " << objWidth << std::endl;

				for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
				{
					std::cout << "node Object Group has attribute " << attr->name() << " ";
					std::cout << "with value " << attr->value() << "\n";

				}
				node = node->first_node("properties");

				node = node->first_node("property");
				std::cout << "Node Property has value " << node->value() << "\n";
				std::string itemName = node->first_attribute("value")->value();
				std::cout << "Item =  " << itemName << std::endl;
				
				node = node->next_sibling("property");
				std::string itemType = node->first_attribute("value")->value();

				node = nodecopy;

				//todo: Add objects to the map!
				float x = objX * tileScale + offset.x;
				float y = objY * tileScale + offset.y;

				context.currentScene->AddItemToScene(itemName, x, y, itemType, true);

				std::cout << "Call AddItemToScene " + itemName + " :" + itemType  << endl;

			}

			//DOORS!
			if (std::string(node->first_attribute("name")->value()) == "DOOR")
			{

				//add an item from the map
				std::cout << "Door Found!" << std::endl;
				objX = std::atoi(node->first_attribute("x")->value());
				objY = std::atoi(node->first_attribute("y")->value());
				objWidth = 32;
				objHeight = 32;

				std::cout << "X = " << objX << std::endl;
				std::cout << "Y = " << objY << std::endl;
				std::cout << "Height = " << objHeight << std::endl;
				std::cout << "Width = " << objWidth << std::endl;

				for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
				{
					std::cout << "node Object Group has attribute " << attr->name() << " ";
					std::cout << "with value " << attr->value() << "\n";

				}
				node = node->first_node("properties");

				node = node->first_node("property");
				std::cout << "Node Property has value " << node->value() << "\n";
				unsigned int DoorKey = std::atoi(node->first_attribute("value")->value());
				std::cout << "Key Required =  " << DoorKey << std::endl;

				node = node->next_sibling("property");
				unsigned int DoorType = std::atoi(node->first_attribute("value")->value());

				node = nodecopy;

				//todo: Add objects to the map!
				float x = objX * tileScale + offset.x;
				float y = objY * tileScale + offset.y;

				context.currentScene->AddDoorToScene(DoorKey, x, y, true);

				std::cout << "Call AddDoorToScene Key: " + std::to_string(DoorKey) + " DoorType :" + std::to_string(DoorType)  << endl;

			}



			//NPC's from the MAP!
			if (std::string(node->first_attribute("name")->value()) == "NPC")
			{
				// add NPC's from the tiled map
				std::cout << "NPC Found!" << std::endl;
				objX = std::atoi(node->first_attribute("x")->value());
				objY = std::atoi(node->first_attribute("y")->value());
				objWidth = 32;
				objHeight = 32;
				//std::string npcName = node->first_attribute("npcName")->value();
				//std::string npcType = node->first_attribute("npcType")->value();


				std::cout << "X = " << objX << std::endl;
				std::cout << "Y = " << objY << std::endl;
				std::cout << "Height = " << objHeight << std::endl;
				std::cout << "Width = " << objWidth << std::endl;

				
				

				for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
				{
					std::cout << "node Object Group has attribute " << attr->name() << " ";
					std::cout << "with value " << attr->value() << "\n";

				}
				node = node->first_node("properties");
				
				node = node->first_node("property");
				std::cout << "Node Property has value " << node->value() << "\n";
				std::string npcName = node->first_attribute("value")->value();
				std::cout << "NPC Name =  " << npcName << std::endl;

				node = node->next_sibling("property");
				std::string npcType = node->first_attribute("value")->value();
				/*toX = std::atof(node->first_attribute("value")->value());
				node = node->next_sibling("property");
				toY = std::atof(node->first_attribute("value")->value());*/
				
				//copy back the node so we dont mangle it
				node = nodecopy;

				//todo: Add objects to the map!
				float x = objX * tileScale + offset.x;
				float y = objY * tileScale + offset.y;

				if (npcName != "ENEMY")
				{
					context.currentScene->AddNpcToScene(npcName, x, y, npcType, true);
				}
				else
				{
					context.currentScene->AddNpcToScene(npcName, x, y, npcType, true);
				}
				


			}//end checking for NPC's

		}// end checking for objects in the layer
	}//end if checking for object layer

	return tileObjects;
}

std::shared_ptr<MapTiles> TileMapParser::BuildMapTiles(xml_node<>* rootNode)
{
	std::shared_ptr<TileSheets> tileSheetData = BuildTileSheetData(rootNode);

	std::shared_ptr<MapTiles> map = std::make_shared<MapTiles>();

	for (xml_node<>* node = rootNode->last_node("layer"); node; node = node->previous_sibling("layer"))
	{
		std::pair<std::string, std::shared_ptr<Layer>> mapLayer = BuildLayer(node, tileSheetData);
		map->emplace_back(mapLayer);
	}

	return map;
}

std::shared_ptr<TileSheets>
TileMapParser::BuildTileSheetData(xml_node<>* rootNode)
{
	TileSheets tileSheets;

	for (xml_node<>* tilesheetNode
		= rootNode->first_node("tileset");
		tilesheetNode;
		tilesheetNode = tilesheetNode->next_sibling("tileset"))
	{
		TileSheetData tileSheetData;

		int firstid
			= std::atoi(tilesheetNode->first_attribute("firstgid")->value());

		tileSheetData.tileSize.x
			= std::atoi(tilesheetNode->first_attribute("tilewidth")->value());

		tileSheetData.tileSize.y
			= std::atoi(tilesheetNode->first_attribute("tileheight")->value());

		int tileCount
			= std::atoi(tilesheetNode->first_attribute("tilecount")->value());

		tileSheetData.
			columns = std::atoi(
				tilesheetNode->first_attribute("columns")->value()
			);

		tileSheetData.rows = tileCount / tileSheetData.columns;

		xml_node<>* imageNode = tilesheetNode->first_node("image");
		tileSheetData.textureId
			= textureAllocator.Add(
				std::string(imageNode->first_attribute("source")->value()));

		tileSheetData.imageSize.x
			= std::atoi(imageNode->first_attribute("width")->value());
		tileSheetData.imageSize.y
			= std::atoi(imageNode->first_attribute("height")->value());

		
		
		//check for animated tiles
		for (xml_node<>* TilesetTile
			= tilesheetNode->first_node("tile");
			TilesetTile;
			TilesetTile = TilesetTile->next_sibling("tile"))
		{

			if (TilesetTile != nullptr)
			{

				int tile = std::atoi(TilesetTile->first_attribute("id")->value());

				xml_node<>* AnimationNode = TilesetTile->first_node("animation");
				if (AnimationNode != nullptr)
				{
					std::cout << "Animation found for TileID: " << tile << std::endl;
					std::vector<AnimationFrame> AnimationTiles;
					
					//std::vector<std::pair<int, std::pair<int, int>>> AnimationTiles;

					for (xml_node<>* animationFrameNode = AnimationNode->first_node("frame");
						animationFrameNode;
						animationFrameNode = animationFrameNode->next_sibling())
					{
						if (animationFrameNode != nullptr)
						{
							AnimationFrame frame;
							int tileid = std::atoi(animationFrameNode->first_attribute("tileid")->value());
							int duration = std::atoi(animationFrameNode->first_attribute("duration")->value());
							std::cout << "Animation Node Found: ID= ";
							std::cout << tileid;
							std::cout << " Duration= ";
							std::cout << duration << std::endl;

							//can i work out the rect
							int textureX = (tileid) % tileSheetData.columns * tileSheetData.tileSize.x;
							int textureY = (tileid) / tileSheetData.columns * tileSheetData.tileSize.y;

							//set the vecotr of Animation tiles
							//AnimationTiles.push_back(std::make_pair(textureX, textureY));
							frame.x = textureX;
							frame.y = textureY;
							frame.duration = duration;

							AnimationTiles.push_back(frame);

						}

					}
					// add the map now
					AnimationIndex.insert({ tile + firstid, AnimationTiles });
				}
			}
		}
		
		// We store the tile sheets firstid.
		tileSheets.insert(
			std::make_pair(
				firstid, std::make_shared<TileSheetData>(tileSheetData))
				);

	}

	return std::make_shared<TileSheets>(tileSheets);
}

std::pair<std::string, std::shared_ptr<Layer>>
TileMapParser::BuildLayer(xml_node<>* layerNode,
	std::shared_ptr<TileSheets> tileSheets)
{

	TileSet tileSet;
	int firstId = 0;
	std::shared_ptr<Layer> layer = std::make_shared<Layer>();

	int width = std::atoi(layerNode->first_attribute("width")->value());
	int height = std::atoi(layerNode->first_attribute("height")->value());

	xml_node<>* dataNode = layerNode->first_node("data");
	char* mapIndices = dataNode->value();

	std::stringstream fileStream(mapIndices);

	int count = 0;

	std::string line;
	while (fileStream.good())
	{
		std::string substr;
		std::getline(fileStream, substr, ','); // 1

			if (!Utilities::IsInteger(substr)) // 2
			{
				// We remove special characters from the int before parsing
				substr.erase(
					std::remove(substr.begin(), substr.end(), '\r'), substr.end());
				substr.erase(
					std::remove(substr.begin(), substr.end(), '\n'), substr.end());

				//TODO: add additional check to 
				//confirm that the character removals have worked:
			}

		int tileId = std::stoi(substr); // 3

		if (tileId != 0)
		{
			auto itr = tileSet.find(tileId);
			if (itr == tileSet.end())
			{
				std::shared_ptr<TileSheetData> tileSheet;

				for (auto iter = tileSheets->rbegin(); iter != tileSheets->rend(); ++iter)
				{
					if (tileId >= iter->first)
					{
						// We know the tile belongs to this tileset.
						firstId = iter->first;
						tileSheet = iter->second;
						break;
					}
				}

				if (!tileSheet)
				{
					//TODO: output error message.
					continue;
				}

				int textureX = (tileId - firstId) % tileSheet->columns;
				int textureY = (tileId - firstId) / tileSheet->columns;

				std::shared_ptr<TileInfo> tileInfo = std::make_shared<TileInfo>(tileSheet->textureId, tileId, sf::IntRect(textureX * tileSheet->tileSize.x, textureY * tileSheet->tileSize.y, tileSheet->tileSize.x, tileSheet->tileSize.y));

				itr = tileSet.insert(std::make_pair(tileId, tileInfo)).first;
			}

			std::shared_ptr<Tile> tile = std::make_shared<Tile>();

			// Bind properties of a tile from a set.
			tile->properties = itr->second; // 7
			tile->x = count % width ; //removed -1 here as we are missing some tiles
			tile->y = count / width;


			layer->tiles.emplace_back(tile); // 8
		}

		count++;
	}

	const std::string layerName = layerNode->first_attribute("name")->value();
	// We now check if a visible attribute is present and use that. 
	// If no attribute present, layer defaults to visible.
	bool layerVisible = true;
	xml_attribute<>* visibleAttribute = layerNode->first_attribute("visible");
	if (visibleAttribute) // 2
	{
		layerVisible = std::stoi(visibleAttribute->value());
	}
	layer->isVisible = layerVisible;



	return std::make_pair(layerName, layer);
}


	void TileMapParser::AddAnimationComponent(std::shared_ptr<Object> object, const int textureID)
	{

		auto animation = object->AddComponent<C_Animation>();

		const unsigned int frameWidth = 64;
		const unsigned int frameHeight = 64;

		const FacingDirection directions[4] = { FacingDirection::Up, FacingDirection::Left, FacingDirection::Down, FacingDirection::Right };

		/*******************
		 * Idle Animations *
		 *******************/
		const bool idleAnimationLooped = false;

		unsigned int idleYFramePos = 512;

		std::map<FacingDirection, std::shared_ptr<Animation>> idleAnimations;

		for (int i = 0; i < 4; i++)
		{
			std::shared_ptr<Animation> idleAnimation = std::make_shared<Animation>();

			idleAnimation->AddFrame(textureID, 0, idleYFramePos, frameWidth, frameHeight, 0.f, idleAnimationLooped);

			idleAnimations.insert(std::make_pair(directions[i], idleAnimation));

			idleYFramePos += frameHeight;
		}

		animation->AddAnimation(AnimationState::Idle, idleAnimations);


		/**********************
		 * Walking Animations *
		 **********************/
		const bool walkAnimationLooped = true;
		const int walkingFrameCount = 9;
		const float delayBetweenWalkingFramesSecs = 0.1f;

		unsigned int walkingYFramePos = 512;

		std::map<FacingDirection, std::shared_ptr<Animation>> walkingAnimations;

		for (int i = 0; i < 4; i++)
		{
			std::shared_ptr<Animation> walkingAnimation = std::make_shared<Animation>();
			for (int i = 0; i < walkingFrameCount; i++)
			{
				walkingAnimation->AddFrame(textureID, i * frameWidth, walkingYFramePos, frameWidth, frameHeight, delayBetweenWalkingFramesSecs, walkAnimationLooped);
			}

			walkingAnimations.insert(std::make_pair(directions[i], walkingAnimation));

			walkingYFramePos += frameHeight;
		}

		animation->AddAnimation(AnimationState::Walk, walkingAnimations);


		/*************************
		 * Projectile Animations *
		 *************************/
		const bool projectileAnimationLooped = true;
		const int projectileFrameCount = 10;
		const float delayBetweenProjectileFramesSecs = 0.08f;

		std::map<FacingDirection, std::shared_ptr<Animation>> projectileAnimations;

		unsigned int projFrameYPos = 1024;

		for (int i = 0; i < 4; i++)
		{
			std::shared_ptr<Animation> projAnimation = std::make_shared<Animation>();
			for (int i = 0; i < projectileFrameCount; i++)
			{
				projAnimation->AddFrame(textureID, i * frameWidth, projFrameYPos, frameWidth, frameHeight, delayBetweenProjectileFramesSecs, projectileAnimationLooped);
			}
			projectileAnimations.insert(std::make_pair(directions[i], projAnimation));

			projFrameYPos += frameHeight;
		}

		animation->AddAnimation(AnimationState::Projectile, projectileAnimations);

	}
