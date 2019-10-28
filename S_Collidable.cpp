#include "S_Collidable.hpp"
#include "SharedContext.hpp"

S_Collidable::S_Collidable(Quadtree& collisionTree) : collisionTree(collisionTree)
{
    Bitmask defaultCollisions;
    defaultCollisions.SetBit((int)CollisionLayer::Default);
    collisionLayers.insert(std::make_pair(CollisionLayer::Default, defaultCollisions));
    
    collisionLayers.insert(std::make_pair(CollisionLayer::Tile, Bitmask(0))); 
    
    Bitmask playerCollisions;
    playerCollisions.SetBit((int) CollisionLayer::Default);
    playerCollisions.SetBit((int) CollisionLayer::Tile);
	playerCollisions.SetBit((int) CollisionLayer::NPC);
    collisionLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));
    
    Bitmask projectileCollisions;
    projectileCollisions.SetBit((int) CollisionLayer::Tile);
	projectileCollisions.SetBit((int)CollisionLayer::NPC);
    collisionLayers.insert(std::make_pair(CollisionLayer::Projectile, projectileCollisions));

	Bitmask npcCollisions;
	npcCollisions.SetBit((int)CollisionLayer::Tile);
	collisionLayers.insert(std::make_pair(CollisionLayer::NPC, npcCollisions));
}

void S_Collidable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
    for (auto o : objects)
    {
        auto collider = o->GetComponent<C_BoxCollider>();
        if (collider)
        {
            CollisionLayer layer = collider->GetLayer();
            
            auto itr = collidables.find(layer);
            
            if (itr != collidables.end())
            {
                collidables[layer].push_back(collider);
            }
            else
            {
                collidables.insert(std::make_pair(layer,  std::vector<std::shared_ptr<C_BoxCollider>>{collider}));
            }
        }
    }
}

void S_Collidable::ProcessRemovals()
{
    for (auto& layer : collidables)
    {
        auto itr = layer.second.begin();
        while (itr != layer.second.end())
        {
            if ((*itr)->owner->IsQueuedForRemoval())
            {
                itr = layer.second.erase(itr);
            }
            else
            {
                ++itr;
            }
        }
    }
}

void S_Collidable::BGClear()
{
	for (auto& layer : collidables)
	{
		if (layer.first == CollisionLayer::Default || layer.first == CollisionLayer::Tile)
		{


			auto objIterator = layer.second.begin();
			while (objIterator != layer.second.end())
			{
				auto obj = *objIterator;


				objIterator = layer.second.erase(objIterator);



			}
		}
	}
}

void S_Collidable::UpdatePositions(std::vector<std::shared_ptr<Object>>& objects)
{
    for (auto o : objects)
    {
        if(!o->transform->isStatic())
        {
            auto collider = o->GetComponent<C_BoxCollider>();
            
            if(collider)
            {
                collisionTree.UpdatePosition(collider);
            }
        }
    }
}

void S_Collidable::SetQuadTreeBounds(sf::FloatRect bounds)
{
	collisionTree.SetBounds(bounds);
}

void S_Collidable::Resolve()
{
	//Debug::Log("Entered S_Collidable::Resolve");
    for (auto maps = collidables.begin(); maps != collidables.end(); ++maps)
    {
		//objectsColliding.clear();
        // If this layer collides with nothing then no need to perform any furter checks.
        if(collisionLayers[maps->first].GetMask() == 0)
        {
            continue;
        }
        
        for (auto collidable : maps->second)
        {
            // If this collidable is static then no need to check if its colliding with other objects.
            if (collidable->owner->transform->isStatic())
            {
                continue;
            }
            
            std::vector<std::shared_ptr<C_BoxCollider>> collisions = collisionTree.Search(collidable->GetCollidable());
            
            for (auto& collision : collisions)
            {
                // Make sure we do not resolve collisions between the the same object.
                if (collidable->owner->instanceID->Get() == collision->owner->instanceID->Get())
                {
                    continue;
                }
                
                bool layersCollide = collisionLayers[collidable->GetLayer()].GetBit(((int)collision->GetLayer()));
                
                if(layersCollide)
                {
                    Manifold m = collidable->Intersects(collision);
                    
                    if(m.colliding)
                    {
						
#ifdef _DEBUG
						Debug::Log("Entered S_Collidable::Resolve - m.colliding -Collidable = " + std::to_string(collidable->owner->instanceID->Get()) + " collision: " + std::to_string(collision->owner->instanceID->Get()));
						auto string = "Entered S_Collidable::Resolve - m.colliding -Collidable = " + std::to_string(collidable->owner->instanceID->Get()) + " collision: " + std::to_string(collision->owner->instanceID->Get());
						string += "\n";
						auto imgcontext = collidable->owner->context->imguilog;
						imgcontext->mylog.AddLog(string.c_str());
#endif _DEBUG                        
						
						auto collisionPair = objectsColliding.emplace(std::make_pair(collidable, collision));
                        
                        if(collisionPair.second)
                        {
                            collidable->owner->OnCollisionEnter(collision);
                            collision->owner->OnCollisionEnter(collidable);
                        }
                    
                        Debug::DrawRect(collision->GetCollidable(), sf::Color::Red);
                        Debug::DrawRect(collidable->GetCollidable(), sf::Color::Red);
                        
                        if(collision->owner->transform->isStatic())
                        {
                            collidable->ResolveOverlap(m);
                        }
                        else
                        {
                            
                            //TODO: how shall we handle collisions when both objects are not static?
                            // We could implement rigidbodies and mass.
                            collidable->ResolveOverlap(m);
                        }
                        
                    }
                }
            }
        }
    }
}

void S_Collidable::Update()
{
	
#ifdef _DEBUG
    collisionTree.DrawDebug();
#endif    
    
	collisionTree.Clear();
    
    for (auto maps = collidables.begin(); maps != collidables.end(); ++maps)
    {
        for (auto collidable : maps->second)
        {
            collisionTree.Insert(collidable);
        }
    }
    
    Resolve();
	ProcessCollidingObjects();
}

void S_Collidable::ProcessCollidingObjects()
{
    auto itr = objectsColliding.begin();
    while (itr != objectsColliding.end())
    {
        auto pair = *itr;
		Debug::Log(" Called S_Collidable::ProcessCollidingObjects Size of: " + std::to_string(objectsColliding.size()));
		//auto imgcontext = pair.first->owner->context->imguilog;
		//auto string = " Called S_Collidable::ProcessCollidingObjects Size of: " + std::to_string(objectsColliding.size());
		//string += "\n";
		//imgcontext->mylog.AddLog(string.c_str());
		//Debug::LogIM(" S_Collidable", imguilog);
        //std::shared_ptr<C_BoxCollider> first = pair.first;
        //std::shared_ptr<C_BoxCollider> second = pair.second;

		//Debug::Log("S_Collidable::ProcessCollidingObjects obj:");
        
        if(pair.first->owner->IsQueuedForRemoval() || pair.second->owner->IsQueuedForRemoval())
        {
            pair.first->owner->OnCollisionExit(pair.second);
            pair.second->owner->OnCollisionExit(pair.first);
            
           itr = objectsColliding.erase(itr);

        }
        else
        {
            Manifold m = pair.first->Intersects(pair.second);
            
            if (!m.colliding)
            {
                pair.first->owner->OnCollisionExit(pair.second);
                pair.second->owner->OnCollisionExit(pair.first);
                
                itr = objectsColliding.erase(itr);
            }
            else
            {
                pair.first->owner->OnCollisionStay(pair.second);
                pair.second->owner->OnCollisionStay(pair.first);
                
                ++itr;
            }
        }
    
    }
}



