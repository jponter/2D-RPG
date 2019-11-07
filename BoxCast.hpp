#ifndef BoxCast_hpp
#define BoxCast_hpp

#pragma once

#include "Object.hpp"
#include "S_Collidable.hpp"
#include <math.h>
#include "Utilities.hpp"

struct BoxcastResult
{
	Object* collision;
	CollisionLayer layer;
};



class BoxCast
{
public:
	BoxCast(Quadtree& collisions);

	BoxcastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to, float castDistance, int exclusionID = -1);
	BoxcastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to, float castDistance, Tag TargetTag = Tag::Player);
	BoxcastResult Cast(const sf::Vector2f& from, const sf::Vector2f& to, CollisionLayer layer);


private:
	sf::FloatRect BuildRect(const sf::Vector2f& lineOne, const sf::Vector2f& lineTwo);
	sf::FloatRect BuildRectArea(const sf::Vector2f& startPos);
	Quadtree& collisions;
};

#endif // BoxCast_hpp



