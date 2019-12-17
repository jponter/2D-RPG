#pragma once


#include "Door.hpp"
#include <memory>
#include <vector>



class WorldDoors
{
public:
	WorldDoors() {};
	unsigned int Add(std::shared_ptr<Door> door);
	const std::shared_ptr<Door>& Find(unsigned int DoorID);
	std::vector<std::shared_ptr<Door>>& GetWorldDoors();



private:
	std::vector<std::shared_ptr<Door>> doors;
	unsigned int WorldDoorID = 0;
};

