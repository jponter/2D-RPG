#include "Door.hpp"

Door::Door(unsigned int doorID, std::string doorLevel, int textureID, DoorStates doorState, unsigned int doorKey) 
{
	m_thisDoor.doorID = doorID;
	m_thisDoor.doorLevel = doorLevel;
	m_thisDoor.textureID = textureID;
	m_thisDoor.doorState = doorState;
	m_thisDoor.doorKey = doorKey;

}

DoorInfo Door::GetInfo()
{
	return m_thisDoor;
}

unsigned int Door::GetID()
{
	return m_thisDoor.doorID;

}

unsigned int Door::GetWorldDoorID()
{
	return m_thisDoor.worldDoorID;
}

void Door::Unlock()
{
	m_thisDoor.doorState = DoorStates::Open;
}
