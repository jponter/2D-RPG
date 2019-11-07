#include "C_AI.hpp"




void C_AI::Update(float deltaTime)
{
	if (m_CurrentState != nullptr)
	{
		m_CurrentState->Update(deltaTime);
	}
}

void C_AI::ChangeState(std::string name)
{
	//exit current state
	if (m_CurrentState != nullptr)
	{
		m_CurrentState->OnExit();
	}

	//try to find new state from map
	auto iter = m_stateMap.find(name);
	if (iter != m_stateMap.end())
	{
		m_CurrentState = iter->second;
		//enter new state
		m_CurrentState->OnEnter();
		
	}
	else
	{
		Debug::Log("C_AI: Couldnt set AI State to: " + name);
		m_CurrentState = nullptr;
	}
}

void C_AI::RegisterState(std::shared_ptr<AI_State> state)
{
	m_stateMap.emplace(state->GetName(), state);
}
