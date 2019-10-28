#pragma once
#include <SFML/System/Vector2.hpp>





class HeroClass
{

public:
	int health = 100;
	sf::Vector2f pos = { 0, 0 };


	int getHealth()
	{
		return health;
	}

	//std::shared_ptr<Object> player;

};

