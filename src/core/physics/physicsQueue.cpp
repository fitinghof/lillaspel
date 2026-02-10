#include "core/physics/physicsQueue.h"

PhysicsQueue::~PhysicsQueue()
{
}

PhysicsQueue::PhysicsQueue()
{
}

PhysicsQueue& PhysicsQueue::GetInstance()
{
	static PhysicsQueue instance;
	return instance;
}

void PhysicsQueue::AddRigidBody(std::weak_ptr<RigidBody> rigidBody)
{
    rigidBody.lock()->SetId(this->rigidBodyIdCounter);
    this->rigidBodies.push_back(rigidBody);

    this->rigidBodyIdCounter++;
}

void PhysicsQueue::AddStrayCollider(std::weak_ptr<Collider> collider) 
{
    collider.lock()->SetId(this->colliderIdCounter);
    this->strayColliders.push_back(collider);

    this->colliderIdCounter++;
}