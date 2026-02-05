#include "core/physics/collider.h"
#include "core/physics/boxCollider.h"
#include "core/physics/sphereCollider.h"

Collider::Collider()
{
}

Collider::~Collider()
{
}

bool Collider::Collision(Collider* otherCollider)
{
	DirectX::XMFLOAT3 mtvAxis = {};
	float mtvDistance = 0;

	return this->CollisionHandling(otherCollider, mtvAxis, mtvDistance);
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

void Collider::ResolveCollision(DirectX::XMFLOAT3 resolveAxis, float resolveDistance)
{
	//DirectX::XMFLOAT3 resolveVector = FLOAT3MULT1(resolveAxis, resolveDistance);
	//DirectX::XMFLOAT3 newPosition = FLOAT3ADD(this->transform.GetPosition(), resolveVector);
	//this->SetPosition(newPosition);
}

bool Collider::BoxSphereCollision(BoxCollider* box, SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	//using namespace DirectX;

	//XMFLOAT3 boxCenter = box->GetPosition();
	//XMFLOAT3 sphereCenter = sphere->GetPosition();
	//XMVECTOR vSphereCenter = XMLoadFloat3(&sphereCenter);
	//XMFLOAT3 fExtents = FLOAT3MULT1(box->GetExtents(), 1);
	//XMVECTOR vExtents = XMLoadFloat3(&fExtents);

	//XMFLOAT4X4 worldMatrix = box->transform.GetWorldMatrix();
	//XMMATRIX boxWorldMatrix = XMMatrixTranspose(XMLoadFloat4x4(&worldMatrix)); //worldMatrix is pre-transposed, so needs to get un-transposed again here

	////we get the inverse of the rotation and translation matrices, scale should not be included
	//XMVECTOR scale, rotation, translation;
	//XMMatrixDecompose(&scale, &rotation, &translation, boxWorldMatrix);
	////XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(rotation);
	//XMMATRIX rotationMatrix = box->transform.GetRotationMatrix();
	//XMMATRIX invRotationMatrix = XMMatrixTranspose(rotationMatrix); //rotationMatrix here won't be pre-transposed, so a transpose is required to get the inverse
	//XMMATRIX invTranslationMatrix = XMMatrixTranslation(-boxCenter.x, -boxCenter.y, -boxCenter.z);
	//XMMATRIX invScalingMatrix = XMMatrixScaling(1.0f / XMVectorGetX(scale), 1.0f / XMVectorGetY(scale), 1.0f / XMVectorGetZ(scale));

	//XMMATRIX worldToLocalMatrix = invRotationMatrix * invTranslationMatrix;
	//XMVECTOR vLocalSphereCenter = XMVector3TransformCoord(vSphereCenter, worldToLocalMatrix);

	//XMVECTOR closestPointOnBox = XMVectorClamp(vLocalSphereCenter, -vExtents, vExtents);
	//XMVECTOR delta = XMVectorSubtract(vLocalSphereCenter, closestPointOnBox);

	//float distSq = XMVectorGetX(XMVector3LengthSq(delta));
	//float radius = sphere->GetDiameter() * 0.5f;

	//if (distSq < radius * radius)
	//{
	//	XMVECTOR axisLocal = XMVectorZero();
	//	float penetration = 0;

	//	if (box->dynamic)
	//	{
	//		int a = 0;
	//	}

	//	if (distSq < 1e-8f)
	//	{
	//		//this edge case needs to be handled
	//	}
	//	else
	//	{
	//		axisLocal = XMVector3Normalize(delta);
	//		penetration = radius - sqrtf(distSq);
	//	}

	//	XMMATRIX scalingMatrix = XMMatrixScaling(XMVectorGetX(scale), XMVectorGetY(scale), XMVectorGetZ(scale));
	//	XMMATRIX translationMatrix = XMMatrixTranslation(boxCenter.x, boxCenter.y, boxCenter.z);
	//	XMMATRIX localToWorldMatrix = scalingMatrix * rotationMatrix * translationMatrix;
	//	XMVECTOR axisWorld = XMVector3TransformNormal(axisLocal, localToWorldMatrix);
	//	XMStoreFloat3(&resolveAxis, XMVector3Normalize(axisWorld));
	//	resolveDistance = penetration;

	//	//std::cout << "-------------------------------------------------" << std::endl;
	//	//PrintFloat3(sphereCenter, "SphereCenter: ");
	//	//std::cout << "SphereRadius: " << radius << std::endl;
	//	//std::cout << "---" << std::endl;
	//	//PrintFloat3(boxCenter, "BoxCenter: ");
	//	//PrintFloat3(fExtents, "boxExtents(transform.scale)");

	//	//XMFLOAT4X4 world = {};
	//	//XMStoreFloat4x4(&world, boxWorldMatrix);
	//	//std::cout << "worldMatrix: ";
	//	//PrintMatrix(world);
	//	//std::cout << "---" << std::endl;
	//	//std::cout << "penetration: " << penetration << std::endl;
	//	//PrintFloat3(resolveAxis, "resolveAxis: ");
	//	//std::cout << "-------------------------------------------------" << std::endl;

	//	return true;
	}

	//std::cout << "<<<<<not in collision>>>>>" << std::endl;

	return false;
}

DirectX::XMFLOAT3 Collider::GetPosition()
{
	//return this->transform.GetPosition();
}

DirectX::XMFLOAT3 Collider::GetSize()
{
	//return this->transform.GetScale();
}

bool Collider::CollisionHandling(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	//bool collision = false;

	//if (!this->hasInitializedPreviousPosition) //little annoying, is there a way to not do this each check?
	//{
	//	this->previousPosition = this->transform.GetPosition();
	//	this->hasInitializedPreviousPosition = true;
	//}

	//if (GetLengthOfFLOAT3(FLOAT3SUB(this->GetPosition(), this->previousPosition)) > this->shortestExtent * 3) //1.5 shortest side lengths result in coolision detection in 2 steps
	//{
	//	DirectX::XMFLOAT3 finalPosition = this->transform.GetPosition();
	//	DirectX::XMFLOAT3 movementVector = FLOAT3SUB(finalPosition, this->previousPosition);
	//	DirectX::XMFLOAT3 middlePosition = FLOAT3ADD(this->previousPosition, FLOAT3MULT1(movementVector, 0.5f));

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

	//collision = this->DoubleDispatchCollision(otherCollider, mtvAxis, mtvDistance);

	//if (this->type == ColliderType::BOX && otherCollider->type == ColliderType::SPHERE)
	//{
	//	mtvDistance = -mtvDistance;
	//}

	//if (!collision) return false;
	//if (!this->solid || !otherCollider->solid) return collision;

	//// Determine who moves
	//if (this->dynamic && otherCollider->dynamic)
	//{
	//	this->ResolveCollision(mtvAxis, mtvDistance / 2);
	//	otherCollider->ResolveCollision(mtvAxis, -mtvDistance / 2);
	//}
	//else if (this->dynamic)
	//{
	//	this->ResolveCollision(mtvAxis, mtvDistance);
	//}
	//else if (otherCollider->dynamic)
	//{
	//	otherCollider->ResolveCollision(mtvAxis, -mtvDistance);
	//}

	return true;
}
