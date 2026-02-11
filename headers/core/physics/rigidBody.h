#pragma once
#include "gameObjects/gameObject3D.h"
#include "core/physics/collision.h"

class Collider;

class RigidBody : public GameObject3D
{
public:
	RigidBody();
	~RigidBody();

	void Start() override;

	/// <summary>
	/// Loops through children to find children of type collider and puts them in exclusive vector
	/// </summary>
	void SetParent(std::weak_ptr<GameObject> parent) override;
	void AddColliderChild(std::weak_ptr<Collider> collider);
	void RemoveExpiredColliderChild();
	void SetId(int id);

	int GetId();
	int GetNrOfColliderChildren();
	std::vector<std::weak_ptr<Collider>>* GetColliderChildrenVector();

	bool Collision(std::weak_ptr<RigidBody> rigidbody);


private:
	std::vector<std::weak_ptr<Collider>> colliderChildren;
	int id = -1;
};