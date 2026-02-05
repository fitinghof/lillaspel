#pragma once
#include "gameObjects/gameObject3D.h"

class Collider;

class RigidBody : public GameObject3D
{
public:
	RigidBody();
	~RigidBody();

	/// <summary>
	/// Loops through children to find children of type collider and puts them in exclusive vector
	/// </summary>
	void AddColliderChild(std::weak_ptr<Collider> collider);
	void RemoveExpiredColliderChild();
	int GetNrOfColliderChildren();
	std::vector<std::weak_ptr<Collider>>* GetColliderChildrenVector();

	bool Collision(std::weak_ptr<RigidBody> rigidbody);


private:
	std::vector<std::weak_ptr<Collider>> colliderChildren;
};