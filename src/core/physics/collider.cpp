#include "core/physics/collider.h"
#include "core/physics/boxCollider.h"
#include "core/physics/sphereCollider.h"
#include "core/physics/rigidBody.h"
#include "core/physics/physicsQueue.h" //are here to prevent circular dependecies

Collider::Collider()
{

}

Collider::~Collider()
{
}

void Collider::SetParent(std::weak_ptr<GameObject> newParent)
{
	if (newParent.expired())
	{
		Logger::Error("Tried to set expired gameobject as parent");
		return;
	}

	std::shared_ptr<Collider> colliderParent = std::dynamic_pointer_cast<Collider>(newParent.lock());
	if(colliderParent != nullptr)
	{
		Logger::Error("Tried to set Collider as parent to a Collider");
		return;
	}

	this->GameObject3D::SetParent(newParent);
	std::shared_ptr<RigidBody> rigidBodyParent = std::dynamic_pointer_cast<RigidBody>(newParent.lock());
	std::shared_ptr<Collider> thisCollider = std::static_pointer_cast<Collider>(this->GetPtr());

	if (rigidBodyParent)
	{
		PhysicsQueue::GetInstance().AddRigidBody(rigidBodyParent);
		this->castedParent = rigidBodyParent;
		rigidBodyParent->AddColliderChild(thisCollider);
		Logger::Log("Added Rigidbody to physics queue");
	}
	else
	{
		PhysicsQueue::GetInstance().AddStrayCollider(thisCollider);
		Logger::Log("Added stray Collider to physics queue");
	}
}

bool Collider::Collision(Collider* otherCollider)
{
	DirectX::XMFLOAT3 mtvAxis = {};
	float mtvDistance = 0;

	return this->CollisionHandling(otherCollider, mtvAxis, mtvDistance);

	Logger::Log(":::::::::::::::::After CollisionHandling::::::::::::::::");

	std::string o = "mtvAxis: " + std::to_string(mtvAxis.x) + ", " + std::to_string(mtvAxis.y) + ", " + std::to_string(mtvAxis.z);
	Logger::Log(o);

	std::string b = "mtvDistance: " + std::to_string(mtvDistance);
	Logger::Log(b);
}

bool Collider::Collision(Collider* otherCollider, DirectX::XMVECTOR& contactNormal)
{
	DirectX::XMFLOAT3 mtvAxis = {};
	float mtvDistance = 0;
	bool collision = this->CollisionHandling(otherCollider, mtvAxis, mtvDistance);

	contactNormal = DirectX::XMLoadFloat3(&mtvAxis);
	contactNormal = DirectX::XMVector3Normalize(contactNormal);

	return collision;
}

void Collider::Tick()
{
	this->GameObject3D::Tick();
}

void Collider::SetId(int id) 
{
	this->id = id;
}

int Collider::GetId()
{ 
	return this->id; 
}

void Collider::ResolveCollision(DirectX::XMFLOAT3 resolveAxis, float resolveDistance)
{
	std::shared_ptr<RigidBody> rigidBodyParent = this->castedParent.lock();
	GameObject3D* moveTarget = this;

	if (rigidBodyParent)
	{
		moveTarget = rigidBodyParent.get();
	}

	DirectX::XMFLOAT3 resolveVector = FLOAT3MULT1(resolveAxis, resolveDistance);
	DirectX::XMVECTOR moveDistance = DirectX::XMLoadFloat3(&resolveVector);

	DirectX::XMVECTOR oldPosition = moveTarget->transform.GetPosition();
	DirectX::XMVECTOR newPosition = DirectX::XMVectorAdd(oldPosition, moveDistance);

	moveTarget->transform.SetPosition(newPosition);
}

bool Collider::BoxSphereCollision(BoxCollider* box, SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	using namespace DirectX;

	//CHECK MATRIX TRANSPOSE's!

	XMVECTOR boxCenter = box->GetGlobalPosition();
	XMVECTOR sphereCenter = sphere->GetGlobalPosition();
	XMFLOAT3 fExtents = FLOAT3MULT1(box->GetExtents(), 1);
	XMVECTOR vExtents = XMLoadFloat3(&fExtents);

	//inverse transpose?
	//XMMATRIX boxWorldMatrix = XMMatrixTranspose(box->GetGlobalWorldMatrix(false)); //worldMatrix is pre-transposed, so needs to get un-transposed again here
	XMMATRIX boxWorldMatrix = box->GetGlobalWorldMatrix(false);

	//we get the inverse of the rotation and translation matrices, scale should not be included
	XMVECTOR scale, rotation, translation;
	XMMatrixDecompose(&scale, &rotation, &translation, boxWorldMatrix);

	//XMMATRIX rotationMatrix = DirectX::XMMatrixRotationX(rotation.m128_f32[0]) * DirectX::XMMatrixRotationY(rotation.m128_f32[1]) * DirectX::XMMatrixRotationZ(rotation.m128_f32[2]);
	XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.m128_f32[0], rotation.m128_f32[1], rotation.m128_f32[2]);
	XMMATRIX invRotationMatrix = XMMatrixTranspose(rotationMatrix); //rotationMatrix here won't be pre-transposed, so a transpose is required to get the inverse
	XMMATRIX invTranslationMatrix = XMMatrixTranslation(-boxCenter.m128_f32[0], -boxCenter.m128_f32[1], -boxCenter.m128_f32[2]);
	XMMATRIX invScalingMatrix = XMMatrixScaling(1.0f / XMVectorGetX(scale), 1.0f / XMVectorGetY(scale), 1.0f / XMVectorGetZ(scale));

	XMMATRIX worldToLocalMatrix = invRotationMatrix * invTranslationMatrix;
	XMVECTOR vLocalSphereCenter = XMVector3TransformCoord(sphereCenter, worldToLocalMatrix);

	XMVECTOR closestPointOnBox = XMVectorClamp(vLocalSphereCenter, -vExtents, vExtents);
	XMVECTOR delta = XMVectorSubtract(vLocalSphereCenter, closestPointOnBox);

	float distSq = XMVectorGetX(XMVector3LengthSq(delta));
	float radius = sphere->GetDiameter() * 0.5f;

	if (distSq < radius * radius)
	{
		XMVECTOR axisLocal = XMVectorZero();
		float penetration = 0;

		if (box->dynamic)
		{
			int a = 0;
		}

		if (distSq < 1e-8f)
		{
			//this edge case needs to be handled
		}
		else
		{
			axisLocal = XMVector3Normalize(delta);
			penetration = radius - sqrtf(distSq);
		}

		XMMATRIX scalingMatrix = XMMatrixScaling(XMVectorGetX(scale), XMVectorGetY(scale), XMVectorGetZ(scale));
		XMMATRIX translationMatrix = XMMatrixTranslation(boxCenter.m128_f32[0], boxCenter.m128_f32[1], boxCenter.m128_f32[2]);
		XMMATRIX localToWorldMatrix = scalingMatrix * rotationMatrix * translationMatrix;
		XMVECTOR axisWorld = XMVector3TransformNormal(axisLocal, localToWorldMatrix);
		XMStoreFloat3(&resolveAxis, XMVector3Normalize(axisWorld));
		resolveDistance = penetration;

		//std::cout << "-------------------------------------------------" << std::endl;
		//PrintFloat3(sphereCenter, "SphereCenter: ");
		//std::cout << "SphereRadius: " << radius << std::endl;
		//std::cout << "---" << std::endl;
		//PrintFloat3(boxCenter, "BoxCenter: ");
		//PrintFloat3(fExtents, "boxExtents(transform.scale)");

		//XMFLOAT4X4 world = {};
		//XMStoreFloat4x4(&world, boxWorldMatrix);
		//std::cout << "worldMatrix: ";
		//PrintMatrix(world);
		//std::cout << "---" << std::endl;
		//std::cout << "penetration: " << penetration << std::endl;
		//PrintFloat3(resolveAxis, "resolveAxis: ");
		//std::cout << "-------------------------------------------------" << std::endl;

		return true;
	}

	//std::cout << "<<<<<not in collision>>>>>" << std::endl;

	return false;
}

bool Collider::CollisionHandling(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	bool collision = false;

	//if (!this->hasInitializedPreviousPosition) //little annoying, is there a way to not do this each check?
	//{
	//	this->previousPosition = this->transform.GetPosition();
	//	this->hasInitializedPreviousPosition = true;
	//}

	//if (GetLengthOfFLOAT3(FLOAT3SUB(this->GetPosition(), this->previousPosition)) > this->shortestExtent * 3) //1.5 shortest side lengths result in coolision detection in 2 steps
	//{
	//	this happens when when move is bigger than 1.5 sideLengths
	//	DirectX::XMVECTOR finalPosition = this->transform.GetPosition();
	//	DirectX::XMVECTOR movementVector = FLOAT3SUB(finalPosition, this->previousPosition);
	//	DirectX::XMVECTOR middlePosition = FLOAT3ADD(this->previousPosition, FLOAT3MULT1(movementVector, 0.5f));

	//	this->SetPosition(middlePosition);
	//	collision = this->DoubleDispatchCollision(otherCollider, mtvAxis, mtvDistance);

	//	if (this->type == ColliderType::BOX && otherCollider->type == ColliderType::SPHERE)
	//	{
	//		mtvDistance = -mtvDistance;
	//	}

	//	if (collision)
	//	{
	//		if (!this->solid || !otherCollider->solid) return true;

	//		if (this->dynamic && otherCollider->dynamic)
	//		{
	//			this->ResolveCollision(mtvAxis, mtvDistance / 2);
	//			otherCollider->ResolveCollision(mtvAxis, -mtvDistance / 2);
	//		}
	//		else if (this->dynamic)
	//		{
	//			this->ResolveCollision(mtvAxis, mtvDistance);
	//		}
	//		else if (otherCollider->dynamic)
	//		{
	//			otherCollider->ResolveCollision(mtvAxis, -mtvDistance);
	//		}

	//		return true;
	//	}
	//	else
	//	{
	//		this->SetPosition(finalPosition);
	//	}
	//}

	Logger::Log(":::::::::::::::::About to do DoubleDispatchCollision::::::::::::::::");

	collision = this->DoubleDispatchCollision(otherCollider, mtvAxis, mtvDistance);

	if (this->type == ColliderType::BOX && otherCollider->type == ColliderType::SPHERE)
	{
		mtvDistance = -mtvDistance;
	}

	Logger::Log(":::::::::::::::::after DoubleDispatchCollision::::::::::::::::");

	Logger::Log("collision: " + collision);

	if (!collision) return false;
	if (!this->solid || !otherCollider->solid) return collision;

	Logger::Log(":::::::::::::::::Made it past solid checks, resolve will be performed::::::::::::::::");

	// Determine who moves
	if (this->dynamic && otherCollider->dynamic)
	{
		this->ResolveCollision(mtvAxis, mtvDistance / 2);
		otherCollider->ResolveCollision(mtvAxis, -mtvDistance / 2);
	}
	else if (this->dynamic)
	{
		this->ResolveCollision(mtvAxis, mtvDistance);
	}
	else if (otherCollider->dynamic)
	{
		otherCollider->ResolveCollision(mtvAxis, -mtvDistance);
	}

	return true;
}
