#pragma once
#ifndef C_DoorOnCollision_hpp
#define C_DoorOnCollision_hpp


#include "Component.hpp"
#include "C_Collidable.hpp"
#include "Debug.hpp"
#include "WorldDoors.hpp"
#include "Quadtree.hpp"
#include "Object.hpp"
#include "Door.hpp"
#include "Inventory.hpp"

class C_DoorOnCollision: public Component , public C_Collidable
{

public:
	C_DoorOnCollision(Object* owner);

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> owner) override;

	unsigned int worldDoorID;


};

#endif


