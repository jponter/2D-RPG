#include "BoxCast.hpp"
#include <corecrt_math_defines.h>


BoxCast::BoxCast(Quadtree& collisions) : collisions(collisions) {}

BoxcastResult BoxCast::Cast(const sf::Vector2f& from, const sf::Vector2f& to, float castDistance, int exclusionID)
{

	// Build empty boxcast result
	BoxcastResult result;
	result.collision = nullptr;
	result.layer = CollisionLayer::Default;
	float fov = 60.0f;

	// Check if from and to are the same, if true return empty result
	if (from == to)
	{
		return result;
	}

	// Create sf::rect that encompasses line
   //sf::Vector2f size = sf::Vector2f(from.x - to.x, from.y - to.y);
	float sightRadius = castDistance;
	float halfRadius = sightRadius / 2;
	sf::FloatRect collisionArea(from.x - halfRadius, from.y - halfRadius, sightRadius, sightRadius);

	//collisionArea.height = 20;
	//collisionArea.width  = 20
	Debug::DrawRect(collisionArea, sf::Color::Red);

	// Retrieve entities from collision system within the raycast area
	std::vector<std::shared_ptr<C_BoxCollider>> entities = collisions.Search(collisionArea);
	std::shared_ptr<C_BoxCollider> finalTarget;

	// If there are no eneities in the zone return empty result
	if (entities.size() == 0)
	{
		return result;
	}

	float enemyAngle = 0;
	double atanResult = 0;
	// we want to check the collisions now for check the angle between the player and enemy
	float maxdistance = 10000;
	for (auto& e : entities)
	{
		float distance;
		// Check if entity has been excluded from raycast collision
		if (exclusionID == e->owner->instanceID->Get())
		{
			continue;
		}

		//only want to interact with things we are interested in
		if (e->owner->name == "NOTSET")
		{
			continue;
		}

		if (e->owner->IsDead())
		{
			continue;
		}

		sf::Vector2f origin = e->owner->transform->GetPosition();

	

		/*atanResult = atan2(p3.y - p1.y, p3.x - p1.x) - atan2(p2.y - p1.y, p2.x - p1.x);

		if (atanResult < 0) atanResult += M_PI * 2;

		atanResult = atanResult * (180 / M_PI);*/

		atanResult = Utilities::AngleBetweenThreePoints(from, to, origin);

		distance = Utilities::DistanceBetweenTwoPoints(origin, to);


		Debug::Log("Enemy hit with BoxCast with ID:  " + std::to_string(e->owner->instanceID->Get()) + " Angle: " + std::to_string(atanResult));
		Debug::Log("Enemy has a distance of " + std::to_string(distance));
		
		if (atanResult < fov || atanResult > 360 - fov)
		{
			Debug::DrawLine(from, origin, sf::Color::White);
			Debug::DrawLine(to, origin, sf::Color::Cyan);
			//only want enemies in front of us
			if (distance < maxdistance)
			{
				maxdistance = distance;
				finalTarget = e;
			}
		}
		else
		Debug::DrawLine(from, origin, sf::Color::Yellow);


		

		
	}

	if (finalTarget != nullptr)
	{
		result.collision = finalTarget->owner;
		result.layer = finalTarget->GetLayer();
		Debug::Log("Enemey Returned:  " + std::to_string(finalTarget->owner->instanceID->Get()));
		return result;
	}

	//todo: we need to calulate decide if we are going to pass back the nearest or a vector of objects; or maybe a function for both!
	//else return an empty result
	return result;
}

BoxcastResult BoxCast::Cast(const sf::Vector2f& from, const sf::Vector2f& to, float castDistance, Tag TargetTag)
{

	// Build empty boxcast result
	BoxcastResult result;
	result.collision = nullptr;
	result.layer = CollisionLayer::Default;
	float fov = 60.0f;

	// Check if from and to are the same, if true return empty result
	if (from == to)
	{
		return result;
	}

	// Create sf::rect that encompasses line
   //sf::Vector2f size = sf::Vector2f(from.x - to.x, from.y - to.y);
	float sightRadius = castDistance;
	float halfRadius = sightRadius / 2;
	sf::FloatRect collisionArea(from.x - halfRadius, from.y - halfRadius, sightRadius, sightRadius);

	//collisionArea.height = 20;
	//collisionArea.width  = 20
	Debug::DrawRect(collisionArea, sf::Color::Red);

	// Retrieve entities from collision system within the raycast area
	std::vector<std::shared_ptr<C_BoxCollider>> entities = collisions.Search(collisionArea);
	std::shared_ptr<C_BoxCollider> finalTarget;

	// If there are no eneities in the zone return empty result
	if (entities.size() == 0)
	{
		return result;
	}

	float enemyAngle = 0;
	double atanResult = 0;
	// we want to check the collisions now for check the angle between the player and enemy
	float maxdistance = 10000;
	for (auto& e : entities)
	{
		float distance;
		// Check if entity has been excluded from raycast collision
		//Only interested in the player in this call
		if (TargetTag != e->owner->tag->Get())
		{
			continue;
		}

		//only want to interact with things we are interested in
		if (e->owner->name == "NOTSET")
		{
			continue;
		}

		if (e->owner->IsDead())
		{
			continue;
		}

		sf::Vector2f origin = e->owner->transform->GetPosition();



		/*atanResult = atan2(p3.y - p1.y, p3.x - p1.x) - atan2(p2.y - p1.y, p2.x - p1.x);

		if (atanResult < 0) atanResult += M_PI * 2;

		atanResult = atanResult * (180 / M_PI);*/

		atanResult = Utilities::AngleBetweenThreePoints(from, to, origin);

		distance = Utilities::DistanceBetweenTwoPoints(origin, to);


		Debug::Log("Enemy hit with BoxCast with ID:  " + std::to_string(e->owner->instanceID->Get()) + " Angle: " + std::to_string(atanResult));
		Debug::Log("Enemy has a distance of " + std::to_string(distance));

		if (atanResult < fov || atanResult > 360 - fov)
		{
			Debug::DrawLine(from, origin, sf::Color::White);
			Debug::DrawLine(to, origin, sf::Color::Cyan);
			//only want enemies in front of us
			if (distance < maxdistance)
			{
				maxdistance = distance;
				finalTarget = e;
			}
		}
		else
			Debug::DrawLine(from, origin, sf::Color::Yellow);





	}

	if (finalTarget != nullptr)
	{
		result.collision = finalTarget->owner;
		result.layer = finalTarget->GetLayer();
		Debug::Log("Enemey Returned:  " + std::to_string(finalTarget->owner->instanceID->Get()));
		return result;
	}

	//todo: we need to calulate decide if we are going to pass back the nearest or a vector of objects; or maybe a function for both!
	//else return an empty result
	return result;
}



BoxcastResult BoxCast::Cast(const sf::Vector2f& from, const sf::Vector2f& to, CollisionLayer layer)
{
	return BoxcastResult();
}


sf::FloatRect BoxCast::BuildRect(const sf::Vector2f& lineOne, const sf::Vector2f& lineTwo)
{
	float left = (lineOne.x < lineTwo.x) ? lineOne.x + (3* 32 / 2) : lineTwo.x - (3* 32 / 2);
	float top = (lineOne.y < lineTwo.y) ? lineOne.y +32 : lineTwo.y -32;
	float width = fabs(lineOne.x - lineTwo.x);
	float height = fabs(lineOne.y - lineTwo.y);
	if (width == 0)  width = 3 * 64;
	if (height == 0) height = 3 * 64;


	return sf::FloatRect(left, top, width, height);
}

sf::FloatRect BoxCast::BuildRectArea(const sf::Vector2f& startPos)
{
	float left = startPos.x - 3 * 64 /2;
	float top = startPos.y - 3 * 64 /2;
	float width = 3 * 64;
	float height = 3 * 64;


	return sf::FloatRect(left, top, width, height);
}