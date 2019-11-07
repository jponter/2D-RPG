#include "AI_Chase.hpp"



AI_Chase::AI_Chase(std::shared_ptr<C_AI> owner) : AI_State(owner)
{
	m_owner = owner;
	m_Origin = m_owner->owner->transform->GetPosition();
}


void AI_Chase::Update(float deltaTime)
{
	//Debug::Log("AI_Chase::Update");

	//increase the attack timer
	attackTimer += deltaTime;
	
	if (attackTimer > attackDeltaTime)
	{
		attackAvailable = true;
		attackTimer = 0;
	}
	else attackAvailable = false;

	if (!m_owner->owner->IsDead())
	{
		//can set some AI here!
		auto velocity = m_owner->owner->GetComponent<C_Velocity>();


		
		direction = m_owner->owner->GetComponent<C_Direction>();
		//draw a line out from the Enemy

		sf::Vector2i heading = direction->GetHeading(); //returns vector with only one direction set

		const sf::Vector2f& startPoint = m_owner->owner->transform->GetPosition();

		sf::Vector2f endPoint;
		endPoint.x = startPoint.x + (heading.x * interactionDistance);
		endPoint.y = startPoint.y + (heading.y * interactionDistance);
		Debug::DrawLine(startPoint, endPoint, sf::Color::Red);

		//cast a box
		BoxcastResult result = m_owner->owner->context->boxcast->Cast(startPoint, endPoint, interactionDistance * 2,  Tag::Player);

		if (result.collision != nullptr)
		{
			int TargetID = result.collision->instanceID->Get();
			Tag TargetTag = result.collision->tag->Get();

			if (TargetTag == Tag::Player)
			{
				Debug::Log("AI_Chase: Raycast result hit Player: " + std::to_string(TargetID));
				//get the heading to the player and set velocity to reach their position
				sf::Vector2f TargetPos = result.collision->transform->GetPosition();
				sf::Vector2f MyPos = m_owner->owner->transform->GetPosition();
				

				float distance = Magnitude(TargetPos - MyPos);
				auto animation = m_owner->owner->GetComponent<C_Animation>();
				//bool attacked = false;
				
				if (distance < 70.0f && attackAvailable == true)
				{
					//TODO: in range attack  - we will want to only do this once every so often though
					velocity->Set(0, 0);
					animation->SetAnimationState(AnimationState::Thrust);
					attacked = true;
					
					
					/*if (animation->IsFinished())
					{
							animation->SetAnimationState(AnimationState::Idle);
					}*/
					auto playerHealth = result.collision->GetComponent<C_EnemyHealth>();
					if (playerHealth != nullptr)
					{
						playerHealth->Damage(20);
					}



				}
				else
				{
					
						if (distance > 70)
						{
							
							
								//animation->SetAnimationState(AnimationState::Walk);
								sf::Vector2f force = Normalise(TargetPos - MyPos) * chaseSpeed;
								velocity->Set(force);
							
						}

							
						
						else
						{
							
								//animation->SetAnimationState(AnimationState::Idle);
								attacked = false;
								velocity->Set(0, 0);
							
								
											

						
						}
					
				}

			}

		}
		else
		{
			// we have no target anymore - resume patrol
			//change to chase
			auto npcai = m_owner->owner->GetComponent<C_AI>();

			//velocity->Set(0, 0); // set the velocity to zero we want the Patrol AI to handle any movement from here out
			npcai->ChangeState("PATROL");
		}
		
		



	}
}

void AI_Chase::OnEnter()
{
	Debug::Log("AI_Chase::OnEnter");
}

void AI_Chase::OnExit()
{
	Debug::Log("AI_Chase::OnExit");

}