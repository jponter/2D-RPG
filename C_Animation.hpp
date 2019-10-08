#pragma once
#ifndef C_Animation_hpp
#define C_Animation_hpp

#include "Component.hpp"
#include "Animation.hpp"
#include "C_Sprite.hpp"
#include "C_Direction.hpp"

#include <unordered_map>

enum class AnimationState
{
	None,
	Idle,
	Walk,
	Projectile
};

using AnimationList =
std::map<FacingDirection, std::shared_ptr<Animation>>;

class C_Animation : public Component
{
public:
	C_Animation(Object* owner);

	void Awake() override;

	void Update(float deltaTime) override;

	// Add animation to object. We need its state as well 
	// so that we can switch to it.
	void AddAnimation(AnimationState state,
						AnimationList& animationList);

	// Set current animation states.
	void SetAnimationState(AnimationState state);

	// Returns current animation state.
	const AnimationState& GetAnimationState() const;

	void SetAnimationDirection(FacingDirection dir);

	void AddAnimationAction(AnimationState state, FacingDirection dir, int frame, AnimationAction action);

private:
	std::shared_ptr<C_Sprite> sprite; // 1
	
	std::unordered_map<AnimationState, AnimationList> animations;
	std::pair<AnimationState, std::shared_ptr<Animation>> currentAnimation;
	FacingDirection currentDirection;
	std::shared_ptr<C_Direction> direction;

	
};











#endif /* C_Animation_hpp */