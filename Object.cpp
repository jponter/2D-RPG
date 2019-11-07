#include "Object.hpp"

Object::Object(SharedContext* context) : context(context) , queuedForRemoval(false), persistant(false), name("NOTSET"), m_dead(false), respawnOnEnter(true)
{
	transform = AddComponent<C_Transform>();
	instanceID = AddComponent<C_InstanceID>();
	tag = AddComponent<C_Tag>();
}

void Object::Awake()
{
	for (const auto& component : components)
	{
		component->Awake();
	}
}

void Object::Start()
{
	for (const auto& component : components)
	{
		component->Start();
	}
}

void Object::Update(float timeDelta)
{
	for (const auto& component : components)
	{
		component->Update(timeDelta);
	}
}

void Object::LateUpdate(float timeDelta)
{
	for (const auto& component : components)
	{
		component->LateUpdate(timeDelta);
	}
}

void Object::Draw(Window& window)
{
	drawable->Draw(window);
}

void Object::QueueForRemoval()
{
	queuedForRemoval = true;
}

void Object::makePersistant()
{
	persistant = true;
}

bool Object::IsDead()
{
	return m_dead;
}

void Object::MakeDead()
{
	m_dead = true;
}

bool Object::isPersistant()
{
	return persistant;
}

bool Object::IsQueuedForRemoval()
{
	return  queuedForRemoval;
}

std::shared_ptr<C_Drawable> Object::GetDrawable()
{
	return drawable;
}

void Object::OnCollisionEnter(std::shared_ptr<C_BoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionEnter(other);
	}
}

void Object::OnCollisionStay(std::shared_ptr<C_BoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionStay(other);
	}
}

void Object::OnCollisionExit(std::shared_ptr<C_BoxCollider> other)
{
	for (const auto& component : collidables)
	{
		component->OnCollisionExit(other);
	}
}
