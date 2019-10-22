#include "QuadtreeObject.hpp"


QuadtreeObject::QuadtreeObject() : QuadtreeObject(5, 5, 0, { 0.f, 0.f, 1920, 1080 },
	nullptr) {}

QuadtreeObject::QuadtreeObject(int maxObjects, int maxLevels, int level,
	sf::FloatRect bounds, QuadtreeObject* parent)
	: maxObjects(maxObjects), maxLevels(maxLevels),
	level(level), bounds(bounds), parent(parent) {}

void QuadtreeObject::Insert(std::shared_ptr<Object> object)
{
	if (children[0] != nullptr) // 1
	{
		sf::Vector2f pos = object->transform->GetPosition();
		sf::FloatRect rect = { pos.x,pos.y,32,32 };
		int indexToPlaceObject =
			GetChildIndexForObject(rect); // 2

		if (indexToPlaceObject != thisTree) // 3
		{
			children[indexToPlaceObject]->Insert(object);
			return;
		}
	}

	objects.emplace_back(object); // 4

	if (objects.size() > maxObjects &&
		level < maxLevels && children[0] == nullptr) // 5
	{
		Split(); // 6

		auto objIterator = objects.begin(); // 7
		while (objIterator != objects.end())
		{
			auto obj = *objIterator;
			sf::Vector2f pos = object->transform->GetPosition();
			sf::FloatRect rect = { pos.x,pos.y,32,32 };
			int indexToPlaceObject =
				GetChildIndexForObject(rect);

			if (indexToPlaceObject != thisTree)
			{
				children[indexToPlaceObject]->Insert(obj);
				objIterator = objects.erase(objIterator);

			}
			else
			{
				++objIterator;
			}
		}
	}
}

void QuadtreeObject::Remove(std::shared_ptr<Object> object)
{
	sf::Vector2f pos = object->transform->GetPosition();
	sf::FloatRect rect = { pos.x,pos.y,32,32 };

	int index = GetChildIndexForObject(rect);

	if (index == thisTree || children[index] == nullptr)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects.at(i)->instanceID->Get()
				== object->instanceID->Get())
			{
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}
	else
	{
		return children[index]->Remove(object);
	}
}

void QuadtreeObject::Clear()
{
	objects.clear();

	for (int i = 0; i < 4; i++)
	{
		if (children[i] != nullptr)
		{
			children[i]->Clear();
			children[i] = nullptr;
		}
	}
}

std::vector<std::shared_ptr<Object>> QuadtreeObject::Search(const sf::FloatRect& area)
{
	std::vector<std::shared_ptr<Object>> possibleOverlaps; // 1
	Search(area, possibleOverlaps); // 2

	std::vector<std::shared_ptr<Object>> returnList;

	for (auto collider : possibleOverlaps)
	{
		sf::Vector2f pos = collider->transform->GetPosition();
		sf::FloatRect rect = { pos.x,pos.y,32,32 };

		if (area.intersects(rect)) // 3
		{
			returnList.emplace_back(collider); // 4
		}
	}

	return returnList;
}

void QuadtreeObject::Search(const sf::FloatRect& area,
	std::vector<std::shared_ptr<Object>>& overlappingObjects)
{
	overlappingObjects.insert(overlappingObjects.end(), objects.begin(), objects.end()); // 1

	if (children[0] != nullptr)
	{
		int index = GetChildIndexForObject(area); // 2

		if (index == thisTree) // 3
		{
			for (int i = 0; i < 4; i++)
			{
				if (children[i]->GetBounds().intersects(area))
				{
					children[i]->Search(area, overlappingObjects);
				}
			}
		}
		else // 4
		{
			children[index]->Search(area, overlappingObjects);
		}
	}
}

const sf::FloatRect& QuadtreeObject::GetBounds() const
{
	return bounds;
}

int QuadtreeObject::GetChildIndexForObject(const sf::FloatRect& objectBounds)
{
	int index = -1;

	double verticalDividingLine = bounds.left + bounds.width * 0.5f;
	double horizontalDividingLine = bounds.top + bounds.height * 0.5f;

	bool north = objectBounds.top < horizontalDividingLine
		&& (objectBounds.height + objectBounds.top < horizontalDividingLine);
	bool south = objectBounds.top > horizontalDividingLine;
	bool west = objectBounds.left < verticalDividingLine
		&& (objectBounds.left + objectBounds.width < verticalDividingLine);
	bool east = objectBounds.left > verticalDividingLine;

	if (east)
	{
		if (north)
		{
			index = childNE;
		}
		else if (south)
		{
			index = childSE;
		}
	}
	else if (west)
	{
		if (north)
		{
			index = childNW;
		}
		else if (south)
		{
			index = childSW;
		}
	}

	return index;
}

void QuadtreeObject::UpdatePosition(std::shared_ptr<Object> object)
{
	QuadtreeObject* quadTree = this;
	sf::Vector2f pos = object->transform->GetPreviousFramePosition();
	sf::FloatRect rect = { pos.x,pos.y,32,32 };

	const sf::FloatRect& prevObjectRect = rect;

	int index = quadTree->GetChildIndexForObject(prevObjectRect);

	while (index != thisTree && quadTree->children[index] != nullptr)
	{
		quadTree = quadTree->children[index].get();
		index = quadTree->GetChildIndexForObject(prevObjectRect);
	}

	for (int i = 0; i < quadTree->objects.size(); i++)
	{
		if (quadTree->objects.at(i)->instanceID->Get() == object->instanceID->Get())
		{
			sf::Vector2f pos = object->transform->GetPosition();
			sf::FloatRect rect = { pos.x,pos.y,32,32 };
			const sf::FloatRect& objectRect = rect;

			if (!quadTree->GetBounds().intersects(objectRect))
			{
				quadTree->Remove(object);

				quadTree = quadTree->parent;
				while (quadTree != nullptr && !quadTree->GetBounds().intersects(objectRect))
				{
					quadTree = quadTree->parent;
				}

				if (quadTree != nullptr)
				{
					quadTree->Insert(object);
				}
			}

			break;
		}
	}
}


void QuadtreeObject::Split()
{
	const int childWidth = bounds.width / 2;
	const int childHeight = bounds.height / 2;

	children[childNE] = std::make_shared<QuadtreeObject>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left + childWidth, bounds.top, childWidth, childHeight),
		this);
	children[childNW] = std::make_shared<QuadtreeObject>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left, bounds.top, childWidth, childHeight),
		this);
	children[childSW] = std::make_shared<QuadtreeObject>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left, bounds.top + childHeight, childWidth, childHeight),
		this);
	children[childSE] = std::make_shared<QuadtreeObject>(maxObjects, maxLevels, level + 1,
		sf::FloatRect(bounds.left + childWidth, bounds.top + childHeight, childWidth, childHeight),
		this);
}

void QuadtreeObject::DrawDebug()
{
	if (children[0] != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			children[i]->DrawDebug();
		}
	}

	Debug::DrawRect(bounds, sf::Color::Red);

}