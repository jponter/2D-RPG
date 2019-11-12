#pragma once

#include "Object.hpp"
#include "boost/assign/list_of.hpp"

enum class  ItemTypes
{
	HEALTH = 1
};


class Item 
{

public:
	Item(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem);
	std::string GetName();
	std::string GetData();
	void Increase();
	

private:
	std::string mName;
	std::string mDescription;
	std::string m_itemData;
	bool mbKeyItem;
	int m_textureID;
	int m_row;
	int m_column;
	ItemTypes m_itemType;
	int m_count = 1;


};



