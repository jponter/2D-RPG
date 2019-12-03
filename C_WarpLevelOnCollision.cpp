#include "C_WarpLevelOnCollision.hpp"
#include "Object.hpp"
#include "Debug.hpp"
#include "SharedContext.hpp"
#include "SceneDungeon.hpp"


C_WarpLevelOnCollision::C_WarpLevelOnCollision(Object* owner) : Component(owner) {}

void C_WarpLevelOnCollision::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	auto layer = other->GetLayer();
	
	if (layer == CollisionLayer::Player)
	{
		
		Debug::Log("Warp Collision - from Warp Tile ID: " + std::to_string(owner->instanceID->Get()));
		//owner->context->hero->health -= 1; // No Longer testing the health dmg from warping also we should be using the correct dmg
		// modifiers if we want to dmg the player!
			sf::Vector2f pos = this->owner->transform->GetPosition();
			Debug::Log("collision at x: " + std::to_string(owner->transform->GetPosition().x) + "y: " + std::to_string(owner->transform->GetPosition().y));
			//owner->context->currentScene->ChangeLevel1(1);
			//velocity = owner->context->player->GetComponent<C_Velocity>();
			
			owner->context->currentScene->ChangeLevel1(this->warplevel, toX, toY);

			std::cout << "Warping to X:" << toX << " Y: " << toY << std::endl;
			//velocity->Set(0, 0);
			//owner->context->currentScene->
			//owner->context->player->transform->SetPosition(toX, toY);
			
			
	}
}