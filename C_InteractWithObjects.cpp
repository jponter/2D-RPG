#include "C_InteractWithObjects.hpp"
#include "Object.hpp"
#include "S_Quests.hpp"
#include "SceneDungeon.hpp"

C_InteractWithObjects::C_InteractWithObjects(Object* owner) : Component(owner), interactionDistance(100.f) {}

void C_InteractWithObjects::Awake()
{
    direction = owner->GetComponent<C_Direction>();
	animation = owner->GetComponent<C_Animation>();
	
}

void C_InteractWithObjects::Update(float deltaTime)
{
	
	if (owner->context->player->userMovementEnabled)
	{


		/* ENABLE THIS TO DO CONSTANT SIGHT CHECKS
		//draw a line out from the player
		sf::Vector2i heading = direction->GetHeading(); //returns vector with only one direction set

		const sf::Vector2f& startPoint = owner->transform->GetPosition();

		sf::Vector2f endPoint;
		endPoint.x = startPoint.x + (heading.x * interactionDistance * 1.5);
		endPoint.y = startPoint.y + (heading.y * interactionDistance * 1.5);
		Debug::DrawLine(startPoint, endPoint, sf::Color::Red);
		BoxcastResult result = owner->context->boxcast->Cast(startPoint, endPoint, owner->instanceID->Get());
		*/
		

		

		

		

		


		if (owner->context->currentScene->inDialog())
		{
			//skip a frame
		}
		else // process normally
		{
			
			if (owner->context->input->IsKeyDown(Input::Key::R))
			{
				Debug::Log("BoxCast Called");
				//draw a line out from the player
				sf::Vector2i heading = direction->GetHeading(); //returns vector with only one direction set

				const sf::Vector2f& startPoint = owner->transform->GetPosition();

				sf::Vector2f endPoint;
				endPoint.x = startPoint.x + (heading.x * interactionDistance );
				endPoint.y = startPoint.y + (heading.y * interactionDistance );
				Debug::DrawLine(startPoint, endPoint, sf::Color::Red);


				BoxcastResult result = owner->context->boxcast->Cast(startPoint, endPoint, interactionDistance *2 , owner->instanceID->Get());
			}
			
			
			if (owner->context->input->IsKeyDown(Input::Key::SPACE))
			{


				sf::Vector2i heading = direction->GetHeading();

				const sf::Vector2f& startPoint = owner->transform->GetPosition();

				sf::Vector2f endPoint;

				endPoint.x = startPoint.x + (heading.x * interactionDistance);
				endPoint.y = startPoint.y + (heading.y * interactionDistance);

				BoxcastResult result = owner->context->boxcast->Cast(startPoint, endPoint, interactionDistance *2, owner->instanceID->Get());
				//RaycastResult result = owner->context->raycast->Cast(startPoint, endPoint, owner->instanceID->Get());
				
				
				
				bool defaultInteraction = true;
				bool attackInteraction = true;

				if (result.collision != nullptr)
				{
					int TargetID = result.collision->instanceID->Get();

					Debug::Log("Raycast result hit Object: " + std::to_string(TargetID));
					// Retrieve all interactable components
					auto interactables = result.collision->GetComponents<C_Interactable>();

					// check if the collision is friendly if not we could default to attack

					// we would want to check the quests here

					std::list<S_Quests*>* listQuests = owner->context->listQuests;
					std::list<S_Quests*>::iterator it;



					for (it = listQuests->begin(); it != listQuests->end(); it++)

					{
						S_Quests* tempQuest = *it;
						Debug::Log("On InteractWithObject - Quest Check " + tempQuest->sName);
						if (tempQuest->OnInteration(owner->context->objects, result.collision, owner, owner->context->levelName, S_Quests::TALK))
						{
							defaultInteraction = false;
							attackInteraction = false;
							//m_inDialog = true;

							break;
						}



					}

					if (defaultInteraction) // nothing above called interaction so lets look at the default
					{
						// we will check for default interactions now
						for (auto& interactable : interactables)
						{
							
							if (interactable->OnInteraction(owner))
							{
								attackInteraction = false;
								break;
									//m_inDialog = true;
							}
						}
					}

					// we could probably check map interactions too

					//nothing else to do so attack
					if (attackInteraction)
					{
						int hardcodedDamage = 10; // for now damage is hardcoded
						auto targetHealth = result.collision->GetComponent<C_EnemyHealth>();
						// do an attack - not quite sure how LOL
						Debug::Log("Attack reached - Target: " + to_string(TargetID) );
						animation->SetAnimationState(AnimationState::Slash);

						if (targetHealth)
						{
							if (targetHealth->Damage(hardcodedDamage) == false)
							{
								// target is not dead
								result.collision->transform->Pushback(heading);
								//Debug::Log(" Attack Hit! Target now at " + to_string(targetHealth->Get()) + " total health");
							}
							else
							{
								//Debug::Log("C_InteractWithObjects: Attack Hit. Target Dead!");
								
								//result.collision->QueueForRemoval();  // this removes them completely
							}

						}
					}

				}
				else
				{
					
						
						animation->SetAnimationState(AnimationState::Slash);

					
					/*else if (owner->context->input->IsKeyUp(Input::Key::SPACE))
					{
						animation->SetAnimationState(AnimationState::Idle);
					}*/
				}



			}
			if (animation->IsFinished())
			{
				animation->SetAnimationState(AnimationState::Idle);
			}
		}
		// we skipped a frame - set this back to false
		owner->context->currentScene->SetDialog(false);
	}// no interaction if we have disabled player movement
	
}
