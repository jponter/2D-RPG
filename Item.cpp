#include "Item.hpp"

Item::Item(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem)
{
	mName = name;
	m_textureID = textureID;
	m_column = column;
	m_row = row;
	m_itemType = itemType;
	m_itemData = itemData;
	mbKeyItem = keyItem;




	switch (itemType)
	{
	case ItemTypes::HEALTH:
		mDescription = "Health Potion";
		break;
	
	case ItemTypes::KEY:
		mDescription = "A Key!";
		break;

	
	
	default:
		mDescription = "Default - shouldnt happen!";
		break;



	}// end switch



	//std::map<std::string, ItemTypes> itemMap = boost::assign::map_list_of("Health", ItemTypes::HEALTH);
	//	switch (itemMap[itemType])
	//	{
	//	case npcTypes::SKELETON:
	//		textureID = textureAllocator.Add(context.workingDir->Get() + "Skeleton.png");
	//		EnemyType = npcTypes::SKELETON;
	//		break;
	//	case npcTypes::ORC:
	//		textureID = textureAllocator.Add(context.workingDir->Get() + "Orc.png");
	//		EnemyType = npcTypes::ORC;
	//		break;
	//	}// end switch

}

std::string Item::GetName()
{
	return mName;
}

std::string Item::GetData()
{
	return m_itemData;
}

void Item::Increase()
{
	m_count++;
}

void Item::GetInfo(ItemInfo& info)
{
	info.name = mName;
	info.description = mDescription;
	info.data = m_itemData;
	info.keyitem = mbKeyItem;
	info.textureid = m_textureID;
	info.row = m_row;
	info.column = m_column;
	info.count = m_count;
}
