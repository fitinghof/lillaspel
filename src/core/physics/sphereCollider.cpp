#include "core/physics/sphereCollider.h"
#include "core/physics/boxCollider.h"

SphereCollider::SphereCollider()
{
	this->type = ColliderType::SPHERE;
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Tick()
{
	this->Collider::Tick();

	//force scale to the x-factor to ensure sphere shape
	DirectX::XMVECTOR scale = this->transform.GetScale();
	float diameter = scale.m128_f32[0];
	scale.m128_f32[1] = diameter;
	scale.m128_f32[2] = diameter;
	//scale.m128_f32[3] = diameter;
	this->transform.SetScale(scale);
}

bool SphereCollider::DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	return otherCollider->CollidesWithSphere(this, mtvAxis, mtvDistance);
}

void SphereCollider::SetPosition(DirectX::XMFLOAT3 newCenter)
{
	this->transform.SetPosition(DirectX::XMLoadFloat3(&newCenter));
}

void SphereCollider::SetDiameter(float diameter)
{
	DirectX::XMVECTOR sizeVector;
	sizeVector.m128_f32[0] = diameter;
	sizeVector.m128_f32[1] = diameter;
	sizeVector.m128_f32[2] = diameter;

	this->transform.SetScale(sizeVector);
}

float SphereCollider::GetDiameter()
{
	return this->GetGlobalScale().m128_f32[0];
}

bool SphereCollider::CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	return this->BoxSphereCollision(box, this, resolveAxis, resolveDistance);
}

bool SphereCollider::CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	DirectX::XMVECTOR positionA = this->GetGlobalPosition();
	DirectX::XMVECTOR positionB = sphere->GetGlobalPosition();

	DirectX::XMVECTOR distanceVector = DirectX::XMVectorSubtract(positionB, positionA);
	float squaredDistance = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(distanceVector));

	float radiusSum = (this->GetDiameter() + sphere->GetDiameter()) * 0.5f;
	float squaredRadiusSum = radiusSum * radiusSum;

	if (squaredDistance > squaredRadiusSum)
	{
		resolveAxis = DirectX::XMFLOAT3(0, 1, 0);
		resolveDistance = 0;

		return false;
	}

	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVector));
	DirectX::XMVECTOR axis;

	if (distance > 0.0001f)
	{
		axis = DirectX::XMVectorScale(distanceVector, 1.0f / distance); // normalized direction
	}
	else
	{
		axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //perfect overlap gets garbage axis
	}

	XMStoreFloat3(&resolveAxis, axis);
	resolveDistance = radiusSum - distance; //assuming both radiuses are positive

	return true;
}
