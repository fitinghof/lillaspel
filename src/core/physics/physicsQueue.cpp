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

void PhysicsQueue::SolveCollisions()
{
    //what about filtering?

    for(int i = this->rigidBodies.size(); i++;)
    {
        for (int j = 1; j < this->rigidBodies.size() - i; j++)
        {
            this->rigidBodies[i].lock()->Collision(this->rigidBodies[j]);
        }
    }

    for(int i = this->strayColliders.size(); i++;)
    {
        for (int j = 1; j < this->strayColliders.size() - i; j++)
        {
            this->strayColliders[i].lock()->Collision(this->strayColliders[j].lock().get());
        }
    }
}