#include "C_DoorOnCollision.hpp"


C_DoorOnCollision::C_DoorOnCollision(Object* owner) : Component(owner)
{

}

void C_DoorOnCollision::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	if (other->owner->tag->Get() == Tag::Player)
	{

		Debug::Log("Collided with a door! ID: " + std::to_string(worldDoorID));

		//we know the door is locked but let's get a ptr to the door
		auto& door = other->owner->context->worldDoors->Find(worldDoorID);

		if (door != nullptr) // only if valid door
		{

			DoorStates thisDoor = door->GetInfo().doorState;
			unsigned int doorKey = door->GetInfo().doorKey;

			if (thisDoor == DoorStates::Locked)
			{
				Debug::Log("This door is locked!");

				if (other->owner->context->playerInventory->HasItem("KEY", std::to_string(doorKey)))
				{
					Debug::Log("Player has the Key!");
					
					//remove the Door as we have the Key
					auto collider = owner->GetComponent<C_BoxCollider>();
					owner->context->collisionTree->Remove(collider);
					owner->QueueForRemoval();

					//set the door state to open
					door->Unlock();

				}
				else
				{
					Debug::Log("Player does NOT have the Key!");
				}


			}

		} //end door not found


	}
	else
	{
		Debug::Log("Something other than player collided with a door!");
	}

}
