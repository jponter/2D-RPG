#pragma once
#ifndef AI_State_hpp
#define AI_State_hpp

#include "C_AI.hpp"

#include <string>

class C_AI;

class AI_State
{

public:
	AI_State(std::shared_ptr<C_AI> owner) : m_Owner(owner) {};
		

	virtual void Update(float deltaTime) = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
	virtual std::string GetName() = 0;

protected:
	std::shared_ptr<C_AI> m_Owner;
};

#endif // endif AI_State_hpp
