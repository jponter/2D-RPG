#pragma once

#include <string>


enum class DoorStates
{
	Locked = 1,
	Open = 2
};

struct DoorInfo
{
	unsigned int doorID;
	std::string doorLevel;
	int textureID;
	DoorStates doorState;
	unsigned int worldDoorID;
	unsigned int doorKey;



};


class Door
{

public:
	Door(unsigned int doorID, std::string doorLevel, int textureID, DoorStates doorState, unsigned int doorKey);
	DoorInfo GetInfo();
	unsigned int GetID();
	unsigned int GetWorldDoorID();
	
	void SetWorldID(unsigned int worldDoorID) {	m_thisDoor.worldDoorID = worldDoorID;	}

	void Unlock();

private:
	DoorInfo m_thisDoor;

};

