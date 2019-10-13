#pragma once
#ifndef S_Drawable_hpp
#define S_Drawable_hpp

#include <map>

#include "C_Drawable.hpp"
#include "Object.hpp"

class S_Drawable
{
public:
	void Add(std::vector<std::shared_ptr<Object>>& object);

	void ProcessRemovals();
	void BGClear();

	void Draw(Window& window);

private:
	void Add(std::shared_ptr<Object> object);
	void Sort();
	static bool LayerSort(std::shared_ptr<C_Drawable> a, std::shared_ptr<C_Drawable> b);

	std::map<DrawLayer, std::vector<std::shared_ptr<C_Drawable>>> drawables;

};

#endif /* S_Drawable_hpp */

