#include "HeroClass.h"

bool HeroClass::atMaxHealth()
{
	if (health < maxHealth)
	{
		return false;
	}
	else
	{
		return true;
	}
}
