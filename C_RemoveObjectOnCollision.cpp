#include "C_RemoveObjectOnCollision.hpp"
#include "Object.hpp"
#include "Debug.hpp"
#include "C_EnemyHealth.hpp"
#include "Quadtree.hpp"
#include "C_BoxCollider.hpp"

C_RemoveObjectOnCollisionEnter::C_RemoveObjectOnCollisionEnter(Object* owner) : Component(owner) {}

void C_RemoveObjectOnCollisionEnter::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	// Remove the projectile when it collides with any other object  
	Debug::Log("Remove Object on Collision - " + std::to_string(owner->instanceID->Get()));
	Debug::Log("Collided with: " + std::to_string(other->owner->instanceID->Get()));
	auto enemyDamage = other->owner->GetComponent<C_EnemyHealth>();

	if (enemyDamage != nullptr) {
		enemyDamage->Damage(m_arrowDamage);
		
	}

	// we need to remove it from the quadtree immediately
	auto collider = owner->GetComponent<C_BoxCollider>();
	
	owner->context->collisionTree->Remove(collider);
	owner->QueueForRemoval();
}
