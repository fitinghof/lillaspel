#pragma once
#include "gameObjects/gameObject3D.h"
#include "physics/collider.h"

class RigidBody : public GameObject3D
{
public:
	RigidBody();
	~RigidBody();

	/// <summary>
	/// Loops through children to find children of type collider and puts them in exclusive vector
	/// </summary>
	void AddColliderChildren();
	void RemoveExpiredColliderChildren();
	int GetNrOfColliderChildren();
	std::vector<std::weak_ptr<Collider>>* GetRigidBodyCollidersVectorReference();

	void Collision(std::weak_ptr<RigidBody> rigidbody);


private:
	std::vector<std::weak_ptr<Collider>> colliderChildren;
};