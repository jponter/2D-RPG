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

	//std::shared_ptr<Object> player;

};

