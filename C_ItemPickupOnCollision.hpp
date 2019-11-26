#pragma once
#ifndef C_ItemPickupOnCollision_hpp
#define C_ItemPickupOnCollision_hpp

#include "Component.hpp"
#include "C_Collidable.hpp"
#include "Debug.hpp"
#include "Object.hpp"
#include "Quadtree.hpp"
#include "Item.hpp"
#include "Item_Key.hpp"
#include "Inventory.hpp"

class C_ItemPickupOnCollision: public Component, public C_Collidable
{
public:
	C_ItemPickupOnCollision(Object* owner);

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other) override;

	std::string m_itemName = "NotSet";
	std::string m_itemType;
	std::string m_itemData;
	int m_textureId = -1;
	int column = 0;
	int row = 0;
	bool pickup = false;
	bool keyitem = false;
	ItemTypes itemType;



};

#endif


