#include "core/physics/rigidBody.h"
#include "core/physics/collider.h"

RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::AddColliderChild(std::weak_ptr<Collider> collider)
{
	this->colliderChildren.push_back(collider);
}

void RigidBody::RemoveExpiredColliderChild()
{
	//japp
}

int RigidBody::GetNrOfColliderChildren()
{
	return this->colliderChildren.size();
}

std::vector<std::weak_ptr<Collider>>* RigidBody::GetColliderChildrenVector()
{
	return &this->colliderChildren;
}

bool RigidBody::Collision(std::weak_ptr<RigidBody> rigidbody)
{
	//checking expired pointers should be done before calling this, with RemoveExpiredColliderChildren()

	bool collision = false;
	bool tempCollision = false;

	for (int i = 0; i < this->colliderChildren.size(); i++)
	{
		Collider* thisCollider = this->colliderChildren[i].lock().get(); //make sure this ptr isn't stored in collider

		for (int j = 0; j < rigidbody.lock()->GetNrOfColliderChildren(); j++)
		{
			Collider* otherCollider = (*rigidbody.lock()->GetColliderChildrenVector())[j].lock().get(); //make sure this ptr isn't stored in collider
			tempCollision = thisCollider->Collision(otherCollider);

			if (tempCollision) collision = true;
		}
	}

	return collision;
}
