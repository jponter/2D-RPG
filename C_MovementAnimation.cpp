#include "C_MovementAnimation.hpp"

#include "Object.hpp"

C_MovementAnimation::C_MovementAnimation(Object* owner) : Component(owner) { }

void C_MovementAnimation::Awake()
{
	velocity = owner->GetComponent<C_Velocity>();
	animation = owner->GetComponent<C_Animation>();
}

void C_MovementAnimation::Update(float deltaTime)
{
	if (!owner->IsDead()) // dead - no need to update anything!
	{
		if (animation->GetAnimationState() != AnimationState::Projectile && animation->GetAnimationState() != AnimationState::Slash ) // && animation->GetAnimationState() != AnimationState::Thrust)
		{
			const sf::Vector2f& currentVel = velocity->Get();

			if (currentVel.x != 0.f || currentVel.y != 0.f)
			{
				if (animation->GetAnimationState() == AnimationState::Thrust && animation->IsFinished())
				{
					animation->SetAnimationState(AnimationState::Walk);
				}
				else
					animation->SetAnimationState(AnimationState::Walk);
			}
			else
			{
				if (animation->GetAnimationState() == AnimationState::Thrust && !animation->IsFinished())
				{
					//do nothing still in animation
				}
				else
				{
					animation->SetAnimationState(AnimationState::Idle);
				}
				
			}
		}
	}
}