#include "C_InteractWithObjects.hpp"
#include "Object.hpp"
#include "S_Quests.hpp"
#include "SceneDungeon.hpp"

C_InteractWithObjects::C_InteractWithObjects(Object* owner) : Component(owner), interactionDistance(60.f) {}

void C_InteractWithObjects::Awake()
{
    direction = owner->GetComponent<C_Direction>();
}

void C_InteractWithObjects::Update(float deltaTime)
{
    if (owner->context->input->IsKeyDown(Input::Key::R))
    {
        sf::Vector2i heading = direction->GetHeading();
        
        const sf::Vector2f& startPoint = owner->transform->GetPosition();
        
        sf::Vector2f endPoint;
        
        endPoint.x = startPoint.x + (heading.x * interactionDistance);
        endPoint.y = startPoint.y + (heading.y * interactionDistance);
        
        RaycastResult result = owner->context->raycast->Cast(startPoint, endPoint, owner->instanceID->Get());
		bool defaultInteraction = true;
        
        if(result.collision != nullptr)
        {
			int TargetID = result.collision->instanceID->Get();
			
			Debug::Log("Raycast result hit Object: " + std::to_string(TargetID));
            // Retrieve all interactable components
            auto interactables = result.collision->GetComponents<C_Interactable>();
            
          

			// we would want to check the quests here

			std::list<S_Quests*>* listQuests = owner->context->listQuests;
			std::list<S_Quests*>::iterator it;

			

			for (it = listQuests->begin() ; it != listQuests->end(); it++)
			
			{
				S_Quests* tempQuest = *it;
				Debug::Log("On InteractWithObject - Quest Check " + tempQuest->sName);
				if (tempQuest->OnInteration(owner->context->objects, result.collision, owner, owner->context->levelName, S_Quests::TALK))
				{
					defaultInteraction = false;
					break;
				}
					
				
				
			}

			if (defaultInteraction) // nothing above called interaction so lets look at the default
			{
				// we will check for default interactions now
				for (auto& interactable : interactables)
				{

					interactable->OnInteraction(owner);
				}
			}

        }
        
   }
}
