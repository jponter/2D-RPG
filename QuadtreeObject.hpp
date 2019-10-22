#pragma once
#ifndef QuadTreeObject_hpp
#define QuadTreeObject_hpp

#include <memory>
#include <vector>


#include "Object.hpp"
#include "Debug.hpp"

class QuadtreeObject
{
public:
	QuadtreeObject();
	QuadtreeObject(int maxObjects, int maxLevels, int level,
		sf::FloatRect bounds, QuadtreeObject* parent);

	// Inserts object into our quadtree.
	void Insert(std::shared_ptr<Object> object);

	// Removes object from our quadtree when we no longer need it to collide.
	void Remove(std::shared_ptr<Object> object);

	void UpdatePosition(std::shared_ptr<Object> object);

	// Removes all objects from tree.
	void Clear();

	// Returns vector of colliders that intersect with the search area.
	std::vector<std::shared_ptr<Object>>
		Search(const sf::FloatRect& area);

	// Returns the bounds of this node.
	const sf::FloatRect& GetBounds() const;

	//debug drawing
	void DrawDebug();

private:
	void Search(const sf::FloatRect& area,
		std::vector<std::shared_ptr<Object>>&
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
	QuadtreeObject* parent;
	std::shared_ptr<QuadtreeObject> children[4];

	// Stores objects in this node.
	std::vector<std::shared_ptr<Object>> objects;

	// How deep the current node is from the base node. 
	// The first node starts at 0 and then its child node 	
	// is at level 1 and so on.
	int level;

	// The bounds of this node.
	sf::FloatRect bounds;
};

#endif /* QuadTree_hpp */

