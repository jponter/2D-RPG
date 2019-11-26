#pragma once

#include "Item.hpp"

class Item_MAXHealth :
	public Item
{
public:
	Item_MAXHealth(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem);
	bool OnUse(HeroClass& hero ) override;


};

