#pragma once
#ifndef S_Collidable_hpp
#define S_Collidable_hpp

#include <vector>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "Object.hpp"
#include "Quadtree.hpp"
#include "Bitmask.hpp"
#include "ComponentPairHash.hpp"


class S_Collidable
{
public:
	S_Collidable();

	void Add(std::vector<std::shared_ptr<Object>>& objects);
	void ProcessRemovals();
	void BGClear();
	void Update();
	void Resolve();
	void UpdatePositions(std::vector<std::shared_ptr<Object>>& objects);

private:
	
	//void ProcessCollisions(std::vector<std::shared_ptr<Object>>& first, std::vector<std::shared_ptr<Object>>& second);

	// This is used to store collision layer data i.e. which layers can collide.
	std::unordered_map<CollisionLayer, Bitmask> collisionLayers;

	// The collision system stores all collidables along with their layer.
	std::unordered_map<CollisionLayer, std::vector<std::shared_ptr<C_BoxCollider>>> collidables;

	// The quadtree stores the collidables in a spatial aware structure.
	Quadtree collisionTree;

	std::unordered_set<std::pair<std::shared_ptr<C_BoxCollider>, std::shared_ptr<C_BoxCollider>>, ComponentPairHash> objectsColliding;
	// And add the definition for the new function:
	void ProcessCollidingObjects();
};

#endif /* S_Collidable_hpp */

