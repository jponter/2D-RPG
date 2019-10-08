#include "C_RemoveObjectOnCollision.hpp"
#include "Object.hpp"
#include "Debug.hpp"

C_RemoveObjectOnCollisionEnter::C_RemoveObjectOnCollisionEnter(Object* owner) : Component(owner) {}

void C_RemoveObjectOnCollisionEnter::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	// Remove the projectile when it collides with any other object  
	Debug::Log("Remove Object on Collision - " + std::to_string(owner->instanceID->Get()));
	owner->QueueForRemoval();
}
