#include "TileMapParser.hpp"
#include "Utilities.hpp"


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
				// Set the tiles layer to background for now
				sprite->SetDrawLayer(DrawLayer::Background);
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

			// Add new tile Object to the collection.
			tileObjects.emplace_back(tileObject);
		}
		//decrement the layer count
		layerCount--;
	}

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
			tile->x = count % width - 1;
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