#pragma once
#ifndef C_KeyboardMovement_hpp
#define C_KeyboardMovement_hpp

#include "Component.hpp"
#include "Input.hpp"
#include  "C_Animation.hpp"
#include "C_Velocity.hpp"

class C_KeyboardMovement : public Component
{
public:
	C_KeyboardMovement(Object* owner);

	//void SetInput(Input* input);
	void SetMovementSpeed(float moveSpeed);

	void Update(float deltaTime) override;
	void Awake() override;

private:
	float moveSpeed;
	//Input* input = nullptr;

	// We need to store a reference to the 
	// animation component for now.
	std::shared_ptr<C_Animation> animation;
	std::shared_ptr<C_Velocity> velocity;

};

#endif /* C_KeyboardMovement_hpp */

