#include "gameObjects/gameObject.h"

GameObject::GameObject() : children(), parent()
{

}

const std::vector<std::weak_ptr<GameObject>>& GameObject::GetChildren() const
{
	return this->children;
}

const int GameObject::GetChildCount() const
{
	return this->children.size();
}

const std::weak_ptr<GameObject> GameObject::GetParent() const
{
	return this->parent;
}

void GameObject::SetParent(std::weak_ptr<GameObject> newParent)
{
	if (!this->parent.expired())
	{
		// Remove from children
	}

	if (newParent.expired())
	{
		Logger::Error("Tried to set expired parent.");
		return;
	}

	this->parent = newParent;
	this->parent.lock()->AddChild(this->weakPtr);
}

void GameObject::AddChild(std::weak_ptr<GameObject> newChild)
{
	if (newChild.expired())
		return;

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

DirectX::XMVECTOR GameObject::GetGlobalPosition() const
{
	if (this->parent.expired()) {
		Logger::Log("It works.");
	}
	else {
		Logger::Error("It doesn't work.");
	}

	return DirectX::XMVECTOR();
}

void GameObject::SetWeakPtr(std::weak_ptr<GameObject> yourPtr)
{
	this->weakPtr = yourPtr;
}

