#include "core/physics/sphereCollider.h"
#include "core/physics/boxCollider.h"

SphereCollider::SphereCollider()
{
	this->type = ColliderType::SPHERE;
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	//return otherCollider->CollidesWithSphere(this, mtvAxis, mtvDistance);
}

void SphereCollider::SetPosition(DirectX::XMFLOAT3 newCenter)
{
	//this->transform.SetPosition(newCenter);
}

void SphereCollider::SetDiameter(float diameter)
{
	//this->transform.SetScale(DirectX::XMFLOAT3(diameter, diameter, diameter));
}

float SphereCollider::GetDiameter()
{
	//return this->transform.GetScale().x;
}

bool SphereCollider::CollidesWithBox(BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	//return this->BoxSphereCollision(box, this, resolveAxis, resolveDistance);
}

bool SphereCollider::CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	//DirectX::XMFLOAT3 posA = this->GetPosition();
	//DirectX::XMFLOAT3 posB = sphere->GetPosition();
	//DirectX::XMVECTOR positionA = DirectX::XMLoadFloat3(&posA);
	//DirectX::XMVECTOR positionB = DirectX::XMLoadFloat3(&posB);

	//DirectX::XMVECTOR distanceVector = DirectX::XMVectorSubtract(positionB, positionA);
	//float squaredDistance = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(distanceVector));

	//float radiusSum = (this->GetDiameter() + sphere->GetDiameter()) * 0.5f;
	//float squaredRadiusSum = radiusSum * radiusSum;

	//if (squaredDistance > squaredRadiusSum)
	//{
	//	resolveAxis = DirectX::XMFLOAT3(0, 1, 0);
	//	resolveDistance = 0;

	//	return false;
	//}

	//float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(distanceVector));
	//DirectX::XMVECTOR axis;

	//if (distance > 0.0001f)
	//{
	//	axis = DirectX::XMVectorScale(distanceVector, 1.0f / distance); // normalized direction
	//}
	//else
	//{
	//	axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f); //perfect overlap gets garbage axis
	//}

	//XMStoreFloat3(&resolveAxis, axis);
	//resolveDistance = radiusSum - distance; //assuming both radiuses are positive

	return true;
}
