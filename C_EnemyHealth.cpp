#include "C_EnemyHealth.hpp"
#include "C_Animation.hpp"
#include "Debug.hpp"

C_EnemyHealth::C_EnemyHealth(Object* owner) : Component(owner)
{
}

C_EnemyHealth::~C_EnemyHealth()
{
}

int C_EnemyHealth::Get()
{
	return m_health;
}

void C_EnemyHealth::Set(int health)
{
	m_health = health;
}

void C_EnemyHealth::SetMax(int maxhealth)
{
	m_maxhealth = maxhealth;
}

int C_EnemyHealth::GetMax()
{
	return m_maxhealth;
}

//void C_EnemyHealth::Update(float deltaTime)
//{
//
//
//}

bool C_EnemyHealth::Damage(int damage)
{
	m_health -= damage;
	Debug::Log("C_EnemyHealth::Damage on " + std::to_string(owner->instanceID->Get()) + " for " + std::to_string(damage) + " damage");
	Debug::Log("C_EnemyHealth::Damage total health now " + std::to_string(m_health));

	if (m_health > 0)
	{
		return false;
	}
	else
	{
		//NPC is DEAD!
		//Grab the animation
		auto target = owner->GetComponent<C_Animation>();
		target->SetAnimationState(AnimationState::Dead);

		//kill any velocity
		owner->GetComponent<C_Velocity>()->Set(0, 0);

		/*	auto targetDraw = owner->GetComponent<C_Sprite>();
			targetDraw->SetDrawLayer(DrawLayer::Background);*/
			//Set the object to dead which removes it from being a valid target to most things
		owner->MakeDead();

		Debug::Log("C_EnemyHealth Killed ID: " + std::to_string(owner->instanceID->Get()));

		////Change the objects collision layer
		auto collision = owner->GetComponent<C_Collider>();
		collision->SetLayer(CollisionLayer::DEAD);

		return true;
	}
}