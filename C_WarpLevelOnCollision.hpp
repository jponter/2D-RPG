#pragma once
#ifndef C_WarpLevelOnCollision_hpp
#define C_WarpLevelOnCollision_hpp

#include "Component.hpp"
#include "C_Collidable.hpp"
#include "C_Velocity.hpp"

class C_WarpLevelOnCollision : public Component, public C_Collidable
{
public:
	C_WarpLevelOnCollision(Object* owner);

	void OnCollisionEnter(std::shared_ptr<C_BoxCollider> other) override;


	std::string warplevel;
	float toX;
	float toY;
	

private:
	std::shared_ptr<C_Velocity> velocity;
};

#endif