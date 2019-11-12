#include "Inventory.hpp"

void Inventory::Add(std::shared_ptr<Item> item)
{


	items.emplace_back(item);

}

std::shared_ptr<Item> Inventory::Find(std::string name)
{

	for (auto& item : items)
	{
		if (item->GetName() == name)
		{
			return item;
		}

		return nullptr;
	}

	/*auto itemIterator = items.begin();
	while (itemIterator != items.end())
	{
		auto& item = **itemIterator;
		if (item.GetName == name)
			return item;
	}

	return std::shared_ptr<Item>();*/
}

bool Inventory::Increase(std::string name, std::string itemData)
{
	for (auto& item : items)
	{
		if (item->GetName() == name && item->GetData() == itemData)
		{
			item->Increase();
			return true;
		}

		
	}
	return false;


}

bool Inventory::HasItem(std::string name, std::string itemData)
{
	for (auto& item : items)
	{
		if (item->GetName() == name && item->GetData() == itemData)
		{
			
			return true;
		}


	}
	
	return false;
}

bool Inventory::HasItem(std::string name)
{
	for (auto& item : items)
	{
		if (item->GetName() == name)
		{

			return true;
		}


	}

	return false;
}
