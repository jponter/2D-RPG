#pragma once
#ifndef TileMapParser_hpp
#define TileMapParser_hpp

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <sstream>

#include <rapidxml/rapidxml.hpp>

#include <rapidxml/rapidxml_utils.hpp>
#include "Tile.hpp"
#include "Utilities.hpp"
#include "Object.hpp"
#include "C_Sprite.hpp"
#include "C_BoxCollider.hpp"
#include "SharedContext.hpp"


using namespace rapidxml;

struct TileSheetData
{
	// The texture id will be retrieved by using our texture allocator.
	int textureId; // The id of the tile sets texture. 
	sf::Vector2u imageSize; // The size of the texture.
	int columns; // How many columns in the tile sheet.
	int rows; // How many rows in the tile sheet.
	sf::Vector2u tileSize; // The size of an individual tile.
};


struct Layer
{
	std::vector < std::shared_ptr<Tile>> tiles;
	bool isVisible;
};

struct AnimationFrame
{
	int tileid;
	int x;
	int y;
	float duration;
};


// Stores layer names with layer.
using MapTiles = std::vector<std::pair<std::string, std::shared_ptr<Layer>>>;

// Stores the different tile types that can be used.
using TileSet =
std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>;

//stores the different tile sheets
using TileSheets = std::map<int, std::shared_ptr<TileSheetData>>;


class TileMapParser
{
public:
	TileMapParser(ResourceAllocator<sf::Texture>& textureAllocator,
								SharedContext& context);

	std::vector<std::shared_ptr<Object>>
		Parse(const std::string& file, sf::Vector2i offset);
	float WorldX;
	float WorldY;

private:
	

	std::shared_ptr<TileSheets> BuildTileSheetData(xml_node<>* rootNode);
	std::shared_ptr<MapTiles> BuildMapTiles(xml_node<>* rootNode);
	std::pair<std::string, std::shared_ptr<Layer>> BuildLayer(xml_node<>* layerNode, std::shared_ptr<TileSheets> tileSheets);

	//std::map<int,  std::vector<std::pair<int, int>>> AnimationIndex;
	
	//std::map<int,  std::vector<std::pair<int, int>>>::iterator AnimationIterator;

	std::map<int, std::vector<AnimationFrame>> AnimationIndex;
	std::map<int, std::vector<AnimationFrame>>::iterator AnimationIterator;

	ResourceAllocator<sf::Texture>& textureAllocator;
	SharedContext& context;
};



#endif /* TileMapParser_hpp */

