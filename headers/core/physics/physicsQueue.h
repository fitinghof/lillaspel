#pragma once
#include "core/physics/rigidBody.h"
#include "core/physics/rayCaster.h"
#include <limits>


class PhysicsQueue
{
public:
	PhysicsQueue(PhysicsQueue& other) = delete;
	PhysicsQueue& operator=(const PhysicsQueue&) = delete;

	static PhysicsQueue& GetInstance();

    void AddRigidBody(std::weak_ptr<RigidBody> rigidBody);
    void AddStrayCollider(std::weak_ptr<Collider> collider);
    void SolveCollisions();

    bool castRay(Ray& ray, RayCastData& rayCastData, float maxDistance = (std::numeric_limits<float>::max)());

private:
    PhysicsQueue();
    ~PhysicsQueue();

    int rigidBodyIdCounter = 0;
    int colliderIdCounter = 0;

    std::vector<std::weak_ptr<RigidBody>> rigidBodies;
    std::vector<std::weak_ptr<Collider>> strayColliders;

    RayCaster rayCaster;
};