#pragma once

#include "Object.hpp"
#include "boost/assign/list_of.hpp"
#include "Debug.hpp"

enum class  ItemTypes
{
	HEALTH = 1,
	KEY = 2,
	MAXHEALTH = 3
};

struct ItemInfo
{
	std::string name;
	std::string description;
	std::string data;
	int row;
	int column;
	ItemTypes itemType;
	int textureId;
	int count;
	bool keyItem;
};

class Item 
{

public:
	Item(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem);
	std::string GetName();
	std::string GetData();
	std::string GetDescription();
	void SetDescription(std::string description);
	void Increase();
	void Decrease();
	void GetInfo(ItemInfo& info);
	ItemInfo GetInfo();
	virtual bool  OnUse(HeroClass& hero) { return false; };

	
	

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

	ItemInfo thisItem;


};



