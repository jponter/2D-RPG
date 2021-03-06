#include "C_ItemPickupOnCollision.hpp"

C_ItemPickupOnCollision::C_ItemPickupOnCollision(Object* owner) : Component(owner)
{

}

void C_ItemPickupOnCollision::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	
	if (other->owner->tag->Get() != Tag::Player)
	{
		Debug::Log("Someone shot an item or another NPC touched an item - do NOTHING! ");
		Debug::Log("Red Wizard Shot the Food!");
	}
	else
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
				//TODO: make this into a derived object based on the item type
				//todo: yeah a bit like this :D 18/11/2019
				switch (itemType)
				{
				case ItemTypes::KEY:
				{
					std::shared_ptr<Item> item = std::make_shared<Item_Key>(m_itemName, m_textureId, row, column, itemType, m_itemData, keyitem);
					other->owner->context->playerInventory->Add(item);
					break;
				}

				case ItemTypes::MAXHEALTH:
				{
					std::shared_ptr<Item> item = std::make_shared<Item_MAXHealth>(m_itemName, m_textureId, row, column, itemType, m_itemData, keyitem);
					other->owner->context->playerInventory->Add(item);
					break;
				}

				default:
				{
					std::shared_ptr<Item> item = std::make_shared<Item_Health>(m_itemName, m_textureId, row, column, itemType, m_itemData, keyitem);
					other->owner->context->playerInventory->Add(item);
					break;
				}
				}




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
}
