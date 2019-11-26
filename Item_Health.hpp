#pragma once
#include "Item.hpp"

class Item_Health :
	public Item
{
public:
	Item_Health(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem) : Item(name, textureID, row, column, itemType, itemData, keyItem) {}
	bool OnUse(HeroClass& hero) override ;


};




