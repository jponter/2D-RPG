#include "C_ItemPickupOnCollision.hpp"

C_ItemPickupOnCollision::C_ItemPickupOnCollision(Object* owner) : Component(owner)
{

}

void C_ItemPickupOnCollision::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	Debug::Log("Picked up Item " + m_itemName + " Type: " + m_itemType);

	
	
	
	//add to the inventory or use
	if (pickup)
	{
		

		if (other->owner->context->playerInventory->Increase(m_itemName, m_itemData))
		{

			Debug::Log("Increased item count for " + m_itemName);
			//other->owner->context->playerInventory->Add(item);
		}
		else
		{
			//add to inventory
			Debug::Log("Adding to Inventory");
			//we dont have one of these add it to the inventory
			//Item::Item(std::string name, int textureID, int row, int column, ItemTypes itemType,std::string itemData bool keyItem)
			std::shared_ptr<Item> item = std::make_shared<Item>(m_itemName, m_textureId, row, column, itemType, m_itemData, keyitem);
			other->owner->context->playerInventory->Add(item);

		}

	}
	else
	{
		//use right now
		Debug::Log("Using right now!");

	}
	
	
	//remove the map object
	auto collider = owner->GetComponent<C_BoxCollider>();
	owner->context->collisionTree->Remove(collider);
	owner->QueueForRemoval();

}
