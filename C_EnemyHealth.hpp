#pragma once
#ifndef C_EnemyHealth_hpp
#define C_EnemyHealth_hpp


#include "Component.hpp"
#include "Object.hpp"


class C_EnemyHealth : public Component
{
public:
	C_EnemyHealth(Object* owner);
	~C_EnemyHealth();

	int Get();
	bool Damage(int damage);
	void Set(int health);
	void SetMax(int maxhealth);
	int GetMax();

	//void Update(float deltaTime) override;

private:
	int m_maxhealth = 100;
	int m_health = 100;

};

#endif /* C_EnemyHealth */