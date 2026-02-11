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

    for(int i = rigidBodies.size() - 1; i >= 0; i--)
    {
        std::shared_ptr<RigidBody> thisRigidBody = this->rigidBodies[i].lock();
        if(!thisRigidBody)
        {
            this->rigidBodies.erase(this->rigidBodies.begin() + i);
            continue;
        }

        for (int j = i - 1; j >= 0; j--)
        {
            std::shared_ptr<RigidBody> otherRigidBody = this->rigidBodies[j].lock();
            if(!otherRigidBody)
            {
                this->rigidBodies.erase(this->rigidBodies.begin() + j);
                i--;
                continue;
            }

            thisRigidBody->Collision(otherRigidBody);
        }
    }

    for(int i = strayColliders.size() - 1; i >= 0; i--)
    {
        std::shared_ptr<Collider> thisCollider = this->strayColliders[i].lock();
        if(!thisCollider)
        {
            this->strayColliders.erase(this->strayColliders.begin() + i);
            continue;
        }

        for (int j = i - 1; j >= 0; j--)
        {
            std::shared_ptr<Collider> otherCollider = this->strayColliders[j].lock();
            if(!otherCollider)
            {
                this->strayColliders.erase(this->strayColliders.begin() + j);
                i--;
                continue;
            }

            thisCollider->Collision(otherCollider.get());
        }
    }
}

bool PhysicsQueue::castRay(Ray& ray, RayCastData& rayCastData) { 
    return this->rayCaster.castRay(ray, rayCastData, this->strayColliders);
}