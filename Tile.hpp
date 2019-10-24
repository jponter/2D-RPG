#pragma once
#ifndef Tile_h
#define Tile_h

#include "ResourceAllocator.hpp"
#include <SFML\Graphics\Rect.hpp>

// Stores common tile data.
struct TileInfo
{
	TileInfo() : tileID(-1)
	{
	}

	TileInfo(int textureID, unsigned int tileID, sf::IntRect textureRect)
		: textureID(textureID), tileID(tileID), textureRect(textureRect) { }

	int tileID;
	int textureID;
	sf::IntRect textureRect;
	
};

struct Tile
{
	std::shared_ptr<TileInfo> properties;
	int x;
	int y;
	int warp = -1;
};

#endif /* Tile_h */class Tile



