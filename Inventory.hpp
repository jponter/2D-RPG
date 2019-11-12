#pragma once

#include <memory>
#include <vector>
#include "Item.hpp"

class Inventory
{

public:
	Inventory() {};
	void Add(std::shared_ptr<Item> item);
	std::shared_ptr<Item> Find(std::string name);
	bool Increase(std::string name, std::string itemData);
	bool HasItem(std::string name, std::string itemData);
	bool HasItem(std::string name);



private:
	std::vector<std::shared_ptr<Item>> items;
	//unsigned int itemID;
};

