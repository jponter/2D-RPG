#pragma once
#include "Item.hpp"

class Item_Key : 
	public Item
{
public:
	Item_Key(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem) : Item(name, textureID, row, column, itemType, itemData, keyItem) {};
	bool OnUse(HeroClass& hero) override { return false; };

private:
	bool m_privateKeyVal = true;
};

