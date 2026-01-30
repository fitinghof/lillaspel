#include "gameObjects/gameObject.h"

GameObject::GameObject() : children({}), parent(nullptr)
{
}

const std::vector<GameObject*>& GameObject::GetChildren() const
{
	return this->children;
}

const int& GameObject::GetChildCount() const
{
	return this->children.size();
}

const GameObject* GameObject::GetParent() const
{
	return this->parent;
}

void GameObject::SetParent(GameObject* newParent)
{
	this->parent = newParent;
	newParent->AddChild(this);
}

void GameObject::AddChild(GameObject* newChild)
{
	this->children.push_back(newChild);
}

void GameObject::Start()
{
}

void GameObject::Tick()
{
}

void GameObject::LateTick()
{
}

void GameObject::PhysicsTick()
{
	// TO DO
}

void GameObject::LatePhysicsTick()
{
	// TO DO
}

