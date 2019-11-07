#pragma once
#ifndef AI_Patrol_hpp
#define AI_Patrol_hpp

#include "AI_State.hpp"
#include "Debug.hpp"
#include "C_AI.hpp"
#include "C_Direction.hpp"
#include "BoxCast.hpp"


class AI_Patrol :
	public AI_State
{

public:
	AI_Patrol(std::shared_ptr<C_AI> owner)  ;

	//override behaviours for state
	void Update(float deltaTime) override;
	void OnEnter() override;
	void OnExit() override;
	std::string GetName()  override { return "PATROL"; };


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
	float interactionDistance = 100.0f;



};

#endif