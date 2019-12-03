#include "Item_MaxHealth.hpp"

Item_MAXHealth::Item_MAXHealth(std::string name, int textureID, int row, int column, ItemTypes itemType, std::string itemData, bool keyItem) : Item(name, textureID, row, column, itemType, itemData, keyItem)
{
	//Item(name, textureID, row, column, itemType, itemData, keyItem) {};



	SetDescription("Potion of Max Health! really long text string want to see if this auto wraps in the text box that we have set with tgui!");
}

bool Item_MAXHealth::OnUse(HeroClass& hero)
{
	Debug::Log("Item_MAXHealth Used an Item " + this->GetName());

	if (!hero.atMaxHealth())
	{
		hero.FullHeal();
		Item::Decrease();
		return true;
	}
	else
	{
		return false;
	}

}
