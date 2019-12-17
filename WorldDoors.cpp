#include "WorldDoors.hpp"

unsigned int WorldDoors::Add(std::shared_ptr<Door> door)
{
	doors.emplace_back(door);
	WorldDoorID++; // increment the world door ID (should now be non zero)
	return WorldDoorID;
}

const std::shared_ptr<Door>& WorldDoors::Find(unsigned int WorldDoorID)
{
	for (auto&  Door : doors)
	{
		if (Door->GetWorldDoorID() == WorldDoorID)
		{
			return Door;
		}

		return nullptr;
	}
}

std::vector<std::shared_ptr<Door>>& WorldDoors::GetWorldDoors()
{
	return doors;
}
