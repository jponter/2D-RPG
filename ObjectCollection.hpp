#pragma once
#ifndef ObjectCollection_hpp
#define ObjectCollection_hpp

#include <memory>
#include <vector>

#include "Object.hpp"
#include "S_Drawable.hpp"
#include "S_Collidable.hpp"

class ObjectCollection
{
public:
	void Add(std::shared_ptr<Object> object);
	void Add(std::vector<std::shared_ptr<Object>>& objects);

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	void ProcessNewObjects(); // 1
	void ProcessRemovals();

	int size();

	std::vector<std::shared_ptr<Object>>& get();
	

private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::shared_ptr<Object>> newObjects; // 2
	S_Drawable drawables;
	S_Collidable collidables;
};

#endif /* ObjectCollection_hpp */

