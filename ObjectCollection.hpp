#pragma once
#ifndef ObjectCollection_hpp
#define ObjectCollection_hpp

#include <memory>
#include <vector>

#include "Object.hpp"
#include "S_Drawable.hpp"
#include "S_Collidable.hpp"
#include "QuadtreeObject.hpp"

class ObjectCollection
{
public:
	ObjectCollection(S_Drawable& drawableSystem, S_Collidable& collidableSystem);
	void Add(std::shared_ptr<Object> object);
	void Add(std::vector<std::shared_ptr<Object>>& objects);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);
	void DrawSpecificLayer(Window& window, DrawLayer specificLayer);

	void ProcessNewObjects(); // 1
	void ProcessRemovals();
	void RemoveAllQueued();

	void Clear();

	int size();

	std::vector<std::shared_ptr<Object>>& get();
	//QuadtreeObject worldTree;
	S_Collidable& collidables;

private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> newObjects; // 2
	S_Drawable& drawables;
	
	
};

#endif /* ObjectCollection_hpp */

