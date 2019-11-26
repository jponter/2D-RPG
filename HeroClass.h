#pragma once
#include <SFML/System/Vector2.hpp>





class HeroClass
{


public:
	int health = 100;
	sf::Vector2f pos = { 0, 0 };
	int maxHealth = 100;

	int getHealth()
	{
		return health;
	}

	int getMaxHealth()
	{
		return maxHealth;
	}

	bool atMaxHealth();

	bool Damage(int dmg)
	{
		health -= dmg;
		return true;
	}

	bool Heal(int heal)
	{
		health += heal;
		if( health > maxHealth)	health = maxHealth;
		return true;
	}

	bool FullHeal()
	{
		health = maxHealth;
		return true;
	}


	//std::shared_ptr<Object> player;

};

