#include "AI_Patrol.hpp"
#include "Object.hpp"
#include "C_Transform.hpp"
#include "C_Velocity.hpp"
//#include "C_Direction.hpp"


AI_Patrol::AI_Patrol(std::shared_ptr<C_AI> owner) : AI_State(owner)
{
	m_owner = owner;
	m_Origin = m_owner->owner->transform->GetPosition();
}

void AI_Patrol::Update(float deltaTime)
{
	//Debug::Log("AI_Patrol::Update");
	sf::Vector2f force;
	
	if (!m_owner->owner->IsDead())
	{
		//can set some AI here!
		auto velocity = m_owner->owner->GetComponent<C_Velocity>();
		if (velocity != nullptr)
		{
			if (velocity->Get().x == 0) velocity->Set(30.0f, 20.0f);
		}

		auto currentPosition = m_owner->owner->transform->GetPosition();

		if (currentPosition.x > m_Origin.x + 100)
		{
			//velocity->Set(-30.0f, 0.0f);
			force = Normalise(m_Origin - currentPosition) * 30.0f;
			velocity->Set(force);
		}
		else if (currentPosition.x < m_Origin.x - 100)
		{
			//velocity->Set(30.0f, 0.0f);
			force = Normalise(m_Origin - currentPosition) * 30.0f;
			velocity->Set(force);
		}
		if (currentPosition.y > m_Origin.y + 100)
		{
			//velocity->Set(0, 30.0f);
			force = Normalise(m_Origin - currentPosition) * 30.0f;
			velocity->Set(force);
		}
		else if (currentPosition.y < m_Origin.y - 100)
		{
			//velocity->Set(0, -30.0f);
			force = Normalise(m_Origin - currentPosition) * 30.0f;
			velocity->Set(force);

		}

		

		


		//TODO: check whether a player is in range if so change the AI state
		direction = m_owner->owner->GetComponent<C_Direction>();
		//draw a line out from the Enemy
		
		sf::Vector2i heading = direction->GetHeading(); //returns vector with only one direction set

		const sf::Vector2f& startPoint = m_owner->owner->transform->GetPosition();

		sf::Vector2f endPoint;
		endPoint.x = startPoint.x + (heading.x * interactionDistance );
		endPoint.y = startPoint.y + (heading.y * interactionDistance );
		Debug::DrawLine(startPoint, endPoint, sf::Color::Red);

		//cast a box
		BoxcastResult result = m_owner->owner->context->boxcast->Cast(startPoint, endPoint, interactionDistance *2, Tag::Player);

		if (result.collision != nullptr)
		{
			int TargetID = result.collision->instanceID->Get();
			Tag TargetTag = result.collision->tag->Get();

			if (TargetTag == Tag::Player)
			{
				Debug::Log("AI_Patrol: Raycast result hit Player: " + std::to_string(TargetID));

				//change to chase
				auto npcai = m_owner->owner->GetComponent<C_AI>();

				velocity->Set(0, 0); // set the velocity to zero we want the chase AI to handle any movement from here out
				npcai->ChangeState("CHASE");
			}
		}


	}
}

void AI_Patrol::OnEnter()
{
	Debug::Log("AI_Patrol::OnEnter");
}

void AI_Patrol::OnExit()
{
	Debug::Log("AI_Patrol::OnExit");

}
