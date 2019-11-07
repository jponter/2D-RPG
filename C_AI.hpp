#pragma once
#ifndef C_AI_hpp
#define C_AI_hpp

#include "Component.hpp"
#include <unordered_map>
#include "AI_State.hpp"
#include "Debug.hpp"

class AI_State;

class C_AI : public Component
{
public:
	C_AI(Object* owner) : Component(owner), owner(owner), m_CurrentState(nullptr) {}

	void Update(float deltaTime) override;
	void ChangeState(std::string name);

	void RegisterState( std::shared_ptr<AI_State> state);
	Object* owner;

private:

	//map of states
	std::unordered_map<std::string, std::shared_ptr<AI_State>> m_stateMap;
	//current state
	std::shared_ptr<AI_State> m_CurrentState;
	


};

#endif



