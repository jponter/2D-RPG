#pragma once
#ifndef QuadTree_hpp
#define QuadTree_hpp

#include <memory>
#include <vector>

#include "C_BoxCollider.hpp"
#include "Object.hpp"
#include "Debug.hpp"

class Quadtree
{
public:
	Quadtree();
	Quadtree(int maxObjects, int maxLevels, int level,
		sf::FloatRect bounds, Quadtree* parent);

	// Inserts object into our quadtree.
	void Insert(std::shared_ptr<C_BoxCollider> object);

	// Removes object from our quadtree when we no longer need it to collide.
	void Remove(std::shared_ptr<C_BoxCollider> object);

	void UpdatePosition(std::shared_ptr<C_BoxCollider> object);

	// Removes all objects from tree.
	void Clear();

	// Returns vector of colliders that intersect with the search area.
	std::vector<std::shared_ptr<C_BoxCollider>>
		Search(const sf::FloatRect& area);

	// Returns the bounds of this node.
	const sf::FloatRect& GetBounds() const;

	//debug drawing
	void DrawDebug();

private:
	void Search(const sf::FloatRect& area,
		std::vector<std::shared_ptr<C_BoxCollider>>&
		overlappingObjects);

	// Returns the index for the node that will contain 		
	// the object. -1 is returned if it is this node.
	int GetChildIndexForObject(const sf::FloatRect& objectBounds);

	// Creates the child nodes.
	void Split();

	// We’ll use these as indices in our array of children.
	static const int thisTree = -1;
	static const int childNE = 0;
	static const int childNW = 1;
	static const int childSW = 2;
	static const int childSE = 3;

	int maxObjects;
	int maxLevels;

	// nulptr is this is the base node.
	Quadtree* parent;
	std::shared_ptr<Quadtree> children[4];

	// Stores objects in this node.
	std::vector<std::shared_ptr<C_BoxCollider>> objects;

	// How deep the current node is from the base node. 
	// The first node starts at 0 and then its child node 	
	// is at level 1 and so on.
	int level;

	// The bounds of this node.
	sf::FloatRect bounds;
};

#endif /* QuadTree_hpp */

