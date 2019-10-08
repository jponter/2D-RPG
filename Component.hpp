#ifndef Component_hpp
#define Component_hpp

#include "Window.hpp"

class Object; // 1

class Component
{
public:
	Component(Object* owner) : owner(owner) {}

	virtual void Awake() {};
	virtual void Start() {};

	virtual void Update(float deltaTime) {};
	virtual void LateUpdate(float deltaTime) {};
	//virtual void Draw(Window& window) {};

	Object* owner;

protected:
	
};

#endif /* Component_hpp */

