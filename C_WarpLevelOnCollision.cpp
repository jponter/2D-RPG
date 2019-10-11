#include "C_WarpLevelOnCollision.hpp"
#include "Object.hpp"
#include "Debug.hpp"
#include "SharedContext.hpp"
#include "SceneGame.hpp"


C_WarpLevelOnCollision::C_WarpLevelOnCollision(Object* owner) : Component(owner) {}

void C_WarpLevelOnCollision::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	auto layer = other->GetLayer();
	
	if (layer == CollisionLayer::Player)
	{
		// Remove the projectile when it collides with any other object  
		Debug::Log("Warp Collision - from PLayer ID: " + std::to_string(owner->instanceID->Get()));
			sf::Vector2f pos = this->owner->transform->GetPosition();
			Debug::Log("collision at x: " + std::to_string(owner->transform->GetPosition().x) + "y: " + std::to_string(owner->transform->GetPosition().y));
			//owner->context->currentScene->ChangeLevel1(1);
			owner->context->currentScene->ChangeLevel1(this->warplevel);
	}
}