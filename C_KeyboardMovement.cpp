#include "C_KeyboardMovement.hpp"
#include "Object.hpp"
#include <imgui.h>
#include "SceneGame.hpp"

C_KeyboardMovement::C_KeyboardMovement(Object* owner)
	: Component(owner), moveSpeed(300.0f) {}


void C_KeyboardMovement::Awake()
{
	velocity = owner->GetComponent<C_Velocity>();
	animation = owner->GetComponent<C_Animation>();
}


//void C_KeyboardMovement::SetInput(Input* input)
//{
//	this->input = input;
//}

void C_KeyboardMovement::SetMovementSpeed(float moveSpeed)
{
	this->moveSpeed = moveSpeed;
}

void C_KeyboardMovement::Update(float deltaTime)
{
	
	if (owner->userMovementEnabled && !owner->IsDead()) // we set this via script engine
	{
		
		//TODO: keyboardmovement should not interact with animation component.
		if (animation->GetAnimationState() == AnimationState::Projectile )
		{
			velocity->Set(0.f, 0.f);
			return;
		}

		float xMove = 0.f;
		if (owner->context->input->IsKeyPressed(Input::Key::Left))
		{
			xMove = -moveSpeed;
			if (owner->transform->GetPosition().x < 0)
				xMove = 0;
		}
		else if (owner->context->input->IsKeyPressed(Input::Key::Right))
		{
			xMove = moveSpeed;
			if (owner->transform->GetPosition().x > owner->context->mapParser->WorldX - 64)
				xMove = 0;
		}

		float yMove = 0.f;
		if (owner->context->input->IsKeyPressed(Input::Key::Up))
		{
			yMove = -moveSpeed;
			if (owner->transform->GetPosition().y < 0)
				yMove = 0;
		}
		else if (owner->context->input->IsKeyPressed(Input::Key::Down))
		{
			yMove = moveSpeed;
			if (owner->transform->GetPosition().y > owner->context->mapParser->WorldY - 64)
				yMove = 0;
		}

		velocity->Set(xMove, yMove);

	}
	//if (owner->context->input->IsKeyPressed(Input::Key::R))
	//{
	//	
	//	owner->context->currentScene->ChangeLevel1(1);
	//}

	//if (owner->context->input->IsKeyPressed(Input::Key::T))
	//{

	//	owner->context->currentScene->ChangeLevel1(0);
	//}
	//
}