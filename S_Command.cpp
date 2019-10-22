#include "S_Command.hpp"

S_Command_MoveTo::S_Command_MoveTo(std::shared_ptr<Object>& object, float x, float y, float duration)
	: m_pObject(object)
{
	m_fTargetPosX = x;
	m_fTargetPosY = y;
	m_fTimeSoFar = 0.0f;
	m_fDuration = std::max(duration, 0.001f);
	//m_pObject = object;


}

void S_Command_MoveTo::Start()
{
	sf::Vector2f pos = m_pObject->transform->GetPosition();
		m_fStartPosX = pos.x;
		m_fStartPosY = pos.y;
		m_pObject->userMovementEnabled = false;

}

void S_Command_MoveTo::Update(float fElapsedTime)
{
	m_fTimeSoFar += fElapsedTime;
	float t = m_fTimeSoFar / m_fDuration;
	if (t > 1.0f) t = 1.0f;

	m_pObject->transform->SetX((m_fTargetPosX - m_fStartPosX) * t + m_fStartPosX);
	m_pObject->transform->SetY((m_fTargetPosY - m_fStartPosY) * t + m_fStartPosY);

	std::shared_ptr<C_Velocity> velocity = m_pObject->GetComponent<C_Velocity>();

	sf::Vector2f vel;
	vel.x = (m_fTargetPosX - m_fStartPosX) / m_fDuration;
	vel.y = (m_fTargetPosY - m_fStartPosY) / m_fDuration;

	velocity->Set(vel);

	if (m_fTimeSoFar >= m_fDuration)
	{
		//object has reached destination so stop
		m_pObject->transform->SetX(m_fTargetPosX);
		m_pObject->transform->SetY(m_fTargetPosY);
		velocity->Set(0.0f, 0.0f);
		bCompleted = true;
		m_pObject->userMovementEnabled = true;
	}

}

S_Command_ShowDialog::S_Command_ShowDialog(std::vector<std::string> line, DrawText& DrawTextEngine, Window& window)
	:DrawTextEngine(DrawTextEngine), window(window)
{
	vecLines = line;
}

void S_Command_ShowDialog::Start()
{
	
	DrawTextEngine.showDialog(vecLines, window,sf::Vector2f(0,0));
}

