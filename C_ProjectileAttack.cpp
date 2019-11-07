#include "C_ProjectileAttack.hpp"
#include "Object.hpp"

C_ProjectileAttack::C_ProjectileAttack(Object* owner) :
	Component(owner),
	projectileVelocity(400.f) // velocity defaults to 400
{}

std::unordered_map<FacingDirection, sf::IntRect> C_ProjectileAttack::textureDirectionBindings = {};
std::unordered_map<FacingDirection, sf::Vector2f> C_ProjectileAttack::offsetDirectionBindings = {};
std::unordered_map<FacingDirection, sf::Vector2f> C_ProjectileAttack::velocityDirectionBindings = {};



void C_ProjectileAttack::Awake()
{
	animation = owner->GetComponent<C_Animation>();
	direction = owner->GetComponent<C_Direction>();
}

void C_ProjectileAttack::Update(float deltaTime)
{
	if (owner->context->input->IsKeyDown(Input::Key::E))
	{
		//SpawnProjectile(); //spawn a projectile - NO Longer needed - we have bound the functions to the animation frame below
		animation->SetAnimationState(AnimationState::Projectile);
		
	}
	else if (owner->context->input->IsKeyUp(Input::Key::E))
	{
		animation->SetAnimationState(AnimationState::Idle);
	}
}

//void C_ProjectileAttack::SetInput(Input* input)
//{
//	this->input = input;
//}
//
//void
//C_ProjectileAttack::SetObjectCollection(ObjectCollection* objects)
//{
//	this->objects = objects;
//}
//
//void
//C_ProjectileAttack::SetWorkingDirectory(WorkingDirectory* workingDirectory)
//{
//	this->workingDirectory = workingDirectory;
//}
//
//void
//C_ProjectileAttack::SetTextureAllocator(ResourceAllocator<sf::Texture>* textureAllocator)
//{
//	this->textureAllocator = textureAllocator;
//}

void C_ProjectileAttack::Start()
{
	projectileTextureID =
		owner->context->textureAllocator->Add(owner->context->workingDir->Get()
			+ "LPC/Weapons/arrow.png");
	//arrow texture rects for different directions
	textureDirectionBindings.emplace(FacingDirection::Up, sf::IntRect(0, 0, 64, 64));
	textureDirectionBindings.emplace(FacingDirection::Left, sf::IntRect(64, 0, 64, 64));
	textureDirectionBindings.emplace(FacingDirection::Down, sf::IntRect(128, 0, 64, 64));
	textureDirectionBindings.emplace(FacingDirection::Right, sf::IntRect(192, 0, 64, 64));

	//arrow texture offsets for better placement of initial arrow position
	offsetDirectionBindings.emplace(FacingDirection::Up,
		sf::Vector2f());
	offsetDirectionBindings.emplace(FacingDirection::Left,
		sf::Vector2f(-8.f, 3.f));
	offsetDirectionBindings.emplace(FacingDirection::Down,
		sf::Vector2f(-3.f, 15.f));
	offsetDirectionBindings.emplace(FacingDirection::Right,
		sf::Vector2f(8.f, 3.f));

	//set the velocity bindings for the arrow
	velocityDirectionBindings.emplace(FacingDirection::Up,
		sf::Vector2f(0.f, -1.f));
	velocityDirectionBindings.emplace(FacingDirection::Left,
		sf::Vector2f(-1.f, 0.f));
	velocityDirectionBindings.emplace(FacingDirection::Down,
		sf::Vector2f(0.f, 1.f));
	velocityDirectionBindings.emplace(FacingDirection::Right,
		sf::Vector2f(1.f, 0.f));

	//add the animation actions to specific frame
	animation->AddAnimationAction(
		AnimationState::Projectile, // 1
		FacingDirection::Up, // 2
		9, // 3
		std::bind(&C_ProjectileAttack::SpawnProjectile, this)); // 4

	animation->AddAnimationAction(AnimationState::Projectile, FacingDirection::Left, 9, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	animation->AddAnimationAction(AnimationState::Projectile, FacingDirection::Down, 9, std::bind(&C_ProjectileAttack::SpawnProjectile, this));
	animation->AddAnimationAction(AnimationState::Projectile, FacingDirection::Right, 9, std::bind(&C_ProjectileAttack::SpawnProjectile, this));

}

void C_ProjectileAttack::SpawnProjectile()
{
	FacingDirection faceDir = direction->Get();

	// Create new object.
	std::shared_ptr<Object> projectile = std::make_shared<Object>(owner->context);

	// We add an offset depending on the direction the player is facing.
	projectile->transform->SetPosition(owner->transform->GetPosition() +
		offsetDirectionBindings.at(faceDir));

	//FacingDirection currentDir = direction->Get();

	// Add sprite component to object
	auto projSprite = projectile->AddComponent<C_Sprite>();
	//projSprite->SetTextureAllocator(textureAllocator);
	projSprite->Load(projectileTextureID);
	projSprite->SetDrawLayer(DrawLayer::Entities);
	projSprite->SetSortOrder(100);

	// Use the direction to set the texture rect.
	projSprite->SetTextureRect(textureDirectionBindings.at(faceDir));

	//add the velocity component for the correct direction
	auto velocity = projectile->AddComponent<C_Velocity>();
	velocity->Set(velocityDirectionBindings.at(faceDir) * projectileVelocity);

	//add a collider for the projectiles
	auto collider = projectile->AddComponent<C_BoxCollider>();
	collider->SetSize(32, 32);
	collider->SetLayer(CollisionLayer::Projectile);

	//add a removal component - in this case on collison enter
	projectile->AddComponent<C_RemoveObjectOnCollisionEnter>();

	// Add object to collection
	owner->context->dynamicObjects->Add(projectile);
}