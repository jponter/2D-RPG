#pragma once
#ifndef C_Item_hpp
#define C_Item_hpp

#include "Component.hpp"
#include "Debug.hpp"

class C_Item: public Component
{
public:
	C_Item(Object* owner) : Component(owner), owner(owner) {}



private:
	Object* owner;

};

#endif

