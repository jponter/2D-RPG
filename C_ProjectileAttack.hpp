#pragma once
#ifndef C_ProjectileAttack_hpp
#define C_ProjectileAttack_hpp

#include "Component.hpp"
#include "C_Animation.hpp"
#include "Input.hpp"
#include "ObjectCollection.hpp"
#include "WorkingDirectory.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "C_RemoveObjectOnCollision.hpp"

class C_ProjectileAttack : public Component
{
public:
	C_ProjectileAttack(Object* owner);

	void Awake() override;

	void Update(float deltaTime) override;

	/*void SetInput(Input* input);
	void SetObjectCollection(ObjectCollection* objects);
	void SetWorkingDirectory(WorkingDirectory* workingDirectory);
	void SetTextureAllocator(ResourceAllocator<sf::Texture>* textureAllocator);*/

	void Start() override;
	void SpawnProjectile();

private:
	std::shared_ptr<C_Animation> animation;
	std::shared_ptr<C_Direction> direction;

	static std::unordered_map<FacingDirection, sf::IntRect> textureDirectionBindings;
	static std::unordered_map<FacingDirection, sf::Vector2f> offsetDirectionBindings;
	static std::unordered_map<FacingDirection, sf::Vector2f> velocityDirectionBindings;
	float projectileVelocity;
	/*Input* input;
	ObjectCollection* objects;
	WorkingDirectory* workingDirectory;
	ResourceAllocator<sf::Texture>* textureAllocator;*/

	int projectileTextureID;
};

#endif /* C_ProjectileAttack_hpp */

