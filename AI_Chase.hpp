#pragma once
#ifndef AI_Chase_hpp
#define AI_Chase_hpp

#include "AI_State.hpp"
#include "Debug.hpp"
#include "C_AI.hpp"
#include "C_Direction.hpp"
#include "BoxCast.hpp"
#include "Object.hpp"
#include "C_Transform.hpp"
#include "C_Velocity.hpp"
#include "C_Tag.hpp"
#include "C_Animation.hpp"
#include "C_EnemyHealth.hpp"

class AI_Chase :
	public AI_State
{
public:
	AI_Chase(std::shared_ptr<C_AI> owner);

	//override behaviours for state
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
	std::string GetName()  override { return "CHASE"; };


private:

	//TODO: move to separate class.
	inline sf::Vector2f Normalise(const sf::Vector2f& v)
	{
		float l = Magnitude(v);

		if (l != 0)
		{
			return v / l;
		}

		return v;
	}

	//TODO: move to separate class.
	inline float Magnitude(const sf::Vector2f& v)
	{
		return sqrt((v.x * v.x) + (v.y * v.y));
	}

	std::shared_ptr<C_AI> m_owner;
	sf::Vector2f m_Origin;
	std::shared_ptr<C_Direction> direction;
	float interactionDistance = 300.0f;
	float chaseSpeed = 50.0f;
	float attackTimer = 0.0f;
	bool attackAvailable = false;
	float attackDeltaTime = 2.0f;
	bool attacked = false;


};

#endif



