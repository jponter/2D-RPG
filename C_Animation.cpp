#include "C_Animation.hpp"
#include "Object.hpp"

C_Animation::C_Animation(Object* owner) : Component(owner),
currentAnimation(AnimationState::None, nullptr),
currentDirection(FacingDirection::Down)
{

}

void C_Animation::Awake()
{
	sprite = owner->GetComponent<C_Sprite>();
	direction = owner->GetComponent<C_Direction>();
}

//void C_Animation::SetAnimationDirection(FacingDirection dir)
//{
//	if (currentAnimation.first != AnimationState::None)
//	{
//		currentAnimation.second->SetDirection(dir);
//	}
//}

void C_Animation::SetAnimationDirection(FacingDirection dir)
{
	if (dir != currentDirection)
	{
		currentDirection = dir;

		auto animationList = animations.find(currentAnimation.first);
		if (animationList != animations.end())
		{
			auto animation = animationList->second.find(currentDirection);

			if (animation != animationList->second.end())
			{
				currentAnimation.second = animation->second;
				currentAnimation.second->Reset();
			}
		}
	}
}

void C_Animation::Update(float deltaTime)
{
	//set the animation direction
	SetAnimationDirection(direction->Get());

	if (currentAnimation.first != AnimationState::None)
	{
		bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

		if (newFrame)
		{
			const FrameData* data = currentAnimation.second->GetCurrentFrame();

			sprite->Load(data->id); // 1   

			// We haven’t created this method yet but we’ll do that shortly.
			sprite->SetTextureRect(data->x, data->y, data->width, data->height);
		}
	}
}

void C_Animation::AddAnimation(AnimationState state,
	AnimationList& animationList)
{
	animations.insert(std::make_pair(state, animationList));

	if (currentAnimation.first == AnimationState::None)
	{
		SetAnimationState(state);
	}
}

void C_Animation::SetAnimationState(AnimationState state)
{
	if (currentAnimation.first == state)
	{
		return;
	}

	auto animationList = animations.find(state);
	if (animationList != animations.end())
	{
		auto animation = animationList->second.find(currentDirection);

		if (animation != animationList->second.end())
		{
			currentAnimation.first = animationList->first;
			currentAnimation.second = animation->second;
			currentAnimation.second->Reset();
		}
	}
}

const AnimationState& C_Animation::GetAnimationState() const
{
	// Returns out current animation state. We can use this 
	// to compare the objects current state to a desired state.
	return currentAnimation.first;
}

void C_Animation::AddAnimationAction(AnimationState state, FacingDirection dir, int frame, AnimationAction action)
{
	auto animationList = animations.find(state);

	if (animationList != animations.end())
	{
		auto animation = animationList->second.find(dir);

		if (animation != animationList->second.end())
		{
			animation->second->AddFrameAction(frame, action);
		}
	}
}

bool C_Animation::IsFinished()
{
	if (currentAnimation.first == AnimationState::Slash || currentAnimation.first == AnimationState::Thrust)
	{
		return currentAnimation.second->IsFinished();
	}

	return false;

}