#include "gameObjects/gameObject.h"
#include "scene/scene.h"

GameObject::GameObject() : children(), parent(), scene(nullptr)
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

	Logger::Log("Set Parent.");
}

void GameObject::AddChild(std::weak_ptr<GameObject> newChild)
{
	if (newChild.expired())
		return;

	this->children.push_back(newChild);
}

void GameObject::Start()
{
	//this->scene->CreateGameObjectOfType<GameObject>();
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
		return DirectX::XMVectorSet(0,0,0,0);
	}
	else {
		return this->parent.lock()->GetGlobalPosition();
	}
}

DirectX::XMMATRIX GameObject::GetGlobalWorldMatrix(bool inverseTranspose) const
{
	if (this->parent.expired()) {
		return DirectX::XMMatrixIdentity();
	}
	else {
		return this->parent.lock()->GetGlobalWorldMatrix(inverseTranspose);
	}
}

void GameObject::SetWeakPtr(std::weak_ptr<GameObject> yourPtr)
{
	this->weakPtr = yourPtr;
}

