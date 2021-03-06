#include "ObjectCollection.hpp"

ObjectCollection::ObjectCollection(S_Drawable& drawableSysttem, S_Collidable& collidableSystem) : drawables(drawableSysttem), collidables(collidableSystem) {}

void ObjectCollection::Update(float deltaTime)
{
	for (auto& o : objects)
	{
		o->Update(deltaTime);
	}
	collidables.Update();
}

void ObjectCollection::LateUpdate(float deltaTime)
{
	for (auto& o : objects)
	{
		o->LateUpdate(deltaTime);
	}
}

void ObjectCollection::Draw(Window& window)
{

	drawables.Draw(window);
}

void ObjectCollection::DrawSpecificLayer(Window& window, DrawLayer specificLayer)
{
	drawables.DrawSpecificLayer(window, specificLayer);
}

void ObjectCollection::Add(std::shared_ptr<Object> object)
{
	newObjects.push_back(object);
}

void ObjectCollection::Add(std::vector<std::shared_ptr<Object>>& otherObjects)
{
	newObjects.insert(newObjects.end(), otherObjects.begin(), otherObjects.end());
}

void ObjectCollection::Clear()
{
	Debug::Log("ObjectCollection::Clear()");
	bool removed = false;
	 auto objIterator = objects.begin();
	while (objIterator != objects.end())
	{
		auto& obj = **objIterator;
		if (obj.isPersistant() == false)
		{
			obj.QueueForRemoval();
			//Debug::Log("ObjectCollection::ProcessRemovals Object Queued for Removal - " + std::to_string(obj.instanceID->Get()));
			//Debug::Log("\n ObjectQueuedRemoval = " + std::to_string(obj.IsQueuedForRemoval()) + "\n");
			
				
			objIterator++;
		}
		else
		{
			//Debug::Log("Object persitant " + std::to_string(obj.instanceID->Get()));
			objIterator++;
		}
	}
	Debug::Log("ObjectCollection::RemoveAllQueued()");
	RemoveAllQueued();
}

void ObjectCollection::ProcessNewObjects()
{
	if (newObjects.size() > 0)
	{
		for (const auto& o : newObjects)
		{
			o->Awake();
		}

		for (const auto& o : newObjects)
		{
			o->Start();
		}

		

		objects.insert(objects.end(), newObjects.begin(), newObjects.end());

		drawables.Add(newObjects);
		collidables.Add(newObjects);

		newObjects.clear();
	}
}

void ObjectCollection::ProcessRemovals()
{
	bool removed = false;
	auto objIterator = objects.begin();
	while (objIterator != objects.end())
	{
		auto obj = **objIterator;

		if (obj.IsQueuedForRemoval())
		{
			
			//Debug::Log("ObjectCollection::ProcessRemovals Object Removed - " + std::to_string(obj.instanceID->Get()));
			objIterator = objects.erase(objIterator);
			removed = true;
		}
		else
		{
			++objIterator;
		}
	}

	if (removed)
	{
		drawables.ProcessRemovals();
		collidables.ProcessRemovals();
	}
}

void ObjectCollection::RemoveAllQueued()
{
	
	Debug::Log("ObjectCollection::RemoveAllQueued::drawbles.ProcessRemovals()");
	drawables.ProcessRemovals();
	Debug::Log("ObjectCollection::RemoveAllQueued::collidables.ProcessRemovals()");
	collidables.ProcessRemovals();
	auto objIterator = objects.begin();
	while (objIterator != objects.end())
	{
		auto obj = **objIterator;

		if (obj.IsQueuedForRemoval())
		{

			//Debug::Log("ObjectCollection::ProcessRemovals Object Removed - " + std::to_string(obj.instanceID->Get()));
			objIterator = objects.erase(objIterator);
			
		}
		else
		{
			++objIterator;
		}
	}

	
}

int ObjectCollection::size()
{
	return this->objects.size();
}

std::vector<std::shared_ptr<Object>>& ObjectCollection::get()
{
	return objects;
}
