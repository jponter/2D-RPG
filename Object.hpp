#ifndef Object_hpp
#define Object_hpp

#include "Window.hpp"
#include <vector>
#include "Component.hpp"
#include "C_Transform.hpp"
#include "C_Drawable.hpp"
#include "C_InstanceID.hpp"
#include "SharedContext.hpp"
#include "C_Collidable.hpp"
#include "C_Tag.hpp"

class Object
{
public:
	//add a constructor to all us to instatiate transform variable
	Object(SharedContext* context);

	// Awake is called when object created. Use to ensure 
	// required components are present.
	void Awake();

	// Start is called after Awake method. Use to initialise variables.
	void Start();

	void Update(float deltaTime);
	void LateUpdate(float deltaTime);
	void Draw(Window& window);

	bool IsQueuedForRemoval();
	void QueueForRemoval();

	bool isPersistant();
	void makePersistant();

	bool IsDead();
	void MakeDead();
	void Revive();
	
	


	std::shared_ptr<C_Transform> transform;
	std::shared_ptr<C_Drawable> GetDrawable();
	std::shared_ptr<C_InstanceID> instanceID;
	std::shared_ptr<C_Tag> tag;

	SharedContext* context;
	bool userMovementEnabled = true;
	std::string name = "NOTSET";



	template <typename T> std::shared_ptr<T> AddComponent() // 1
	{
		// This ensures that we only try to add a class the derives 
		// from Component. This is tested at compile time.
		/*static_assert(std::is_base_of<Component, T>::value,
			"T must derive from Component");*/

		// removed assert 

			// Check that we don't already have a component of this type.
			for (auto& exisitingComponent : components)
			{
				// Currently we prevent adding the same component twice. 
				// This may be something we will change in future.
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent); // 2
				}
			}

		// The object does not have this component so we create it and 
		// add it to our list.
		std::shared_ptr<T> newComponent = std::make_shared<T>(this);
		components.push_back(newComponent);

		//check if it is a drawable component if so 
		if (std::dynamic_pointer_cast<C_Drawable>(newComponent))
		{
			drawable = std::dynamic_pointer_cast<C_Drawable>(newComponent);
		}

		// Check if the component inherits from C_Collidable. If it does store it in a separate vector.
		if (std::dynamic_pointer_cast<C_Collidable>(newComponent))
		{
			collidables.push_back(std::dynamic_pointer_cast<C_Collidable>(newComponent));
		}


		return newComponent;
	};

	template <typename T> std::shared_ptr<T> GetComponent()
	{
		static_assert(std::is_base_of<Component, T>::value,
			"T must derive from Component");

			// Check that we don't already have a component of this type.
			for (auto& exisitingComponent : components)
			{
				if (std::dynamic_pointer_cast<T>(exisitingComponent))
				{
					return std::dynamic_pointer_cast<T>(exisitingComponent);
				}
			}

		return nullptr;
	};

	template <typename T> std::vector<std::shared_ptr<T>> GetComponents()
	{
		std::vector<std::shared_ptr<T>> matchingComponents;
		for (auto& exisitingComponent : components)
		{
			if (std::dynamic_pointer_cast<T>(exisitingComponent))
			{
				matchingComponents.emplace_back(std::dynamic_pointer_cast<T>(exisitingComponent));
			}
		}

		return matchingComponents;
	};

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other);
	void OnCollisionStay(std::shared_ptr<C_BoxCollider> other);
	void OnCollisionExit(std::shared_ptr<C_BoxCollider> other);


private:
	std::vector<std::shared_ptr<Component>> components;
	std::shared_ptr<C_Drawable> drawable;
	std::vector<std::shared_ptr<C_Collidable>> collidables;
	bool queuedForRemoval;
	bool m_dead;
	bool respawnOnEnter;
	bool persistant;
	
};

#endif /* Object_hpp */


