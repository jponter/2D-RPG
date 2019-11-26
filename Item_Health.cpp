#include "Item_Health.hpp"

bool Item_Health::OnUse(HeroClass& hero)
{
	if (!hero.atMaxHealth())
	{
		hero.Heal(atoi(this->GetData().c_str()));

		Item::Decrease();
		return true;
	}
	else
	{
		return false;
	}
}