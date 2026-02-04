#include "core/physics/rigidBody.h"

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
}

RigidBody::AddColliderChildren()
{
	for (int i = 0; i < this->children.size(); i++)
	{
		std::weak_ptr<Collider> collider = dynamic_cast<std::weak_ptr<Collider>>();

		if (!collider.expired)
		{
			this->colliderChildren.push_back(collider);
		}
	}
}

void RigidBody::RemoveExpiredColliderChildren()
{
	for (int i = this->colliderChildren.size() - 1; i >= 0; i--)
	{
		if (colliderChildren[i].expired)
		{
			colliderChildren.erase(this->colliderChildren.begin + i);
		}
	}
}

int RigidBody::GetNrOfColliderChildren()
{
	return this->colliderChildren.size();
}

void RigidBody::Collision(std::weak_ptr<RigidBody> rigidbody)
{
	for (int i = 0; i < this->colliderChildren.size(); i++)
	{
		for (int j = 0; j < rigidbody.lock()->GetNrOfColliderChildren(); j++)
		{
			Collider* thisCollider = this->colliderChildren[i].lock();
			Collider* otherCollider = *(rigidbody.lock()->GetRigidBodyCollidersVectorReference())[j]; //dereferencing not worky

			thisCollider->Collision(otherCollider);
		}
	}
}
