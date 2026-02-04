#include "core/physics/boxCollider.h"
#include "core/physics/sphereCollider.h"

BoxCollider::BoxCollider()
{
	//this->type = ColliderType::BOX;
	//this->satData.positionData = new DirectX::XMFLOAT3[8];
	//this->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
	//this->SetRotation(DirectX::XMFLOAT3(0, 0, 0));

	//this->satData.center = DirectX::XMFLOAT3(0, 0, 0);
	//this->satData.nrOfPositions = 8;
	//this->satData.nrOfNormals = 3;
	//this->satData.normalData = this->axis;

	//this->BuildCornersArray(this->satData.positionData);
}

BoxCollider::~BoxCollider()
{
	delete[] this->satData.positionData;
}

bool BoxCollider::CollidesWithBox(class BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	//bool collision = SAT(this->satData, box->satData, resolveAxis, resolveDistance);
	//return collision;
}

bool BoxCollider::CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	//return this->BoxSphereCollision(this, sphere, resolveAxis, resolveDistance);
}

void BoxCollider::BuildCornersArray(DirectX::XMFLOAT3*& positionArray)
{
	//using namespace DirectX;

	//XMFLOAT4X4 floatMatrix = this->transform.GetWorldMatrix();
	//XMMATRIX worldMatrix = XMLoadFloat4x4(&floatMatrix);
	//worldMatrix = XMMatrixTranspose(worldMatrix);

	//for (int i = 0; i < 8; i++)
	//{
	//	XMVECTOR localPosition = XMLoadFloat3(&localBoxCorners[i]);
	//	XMVECTOR worldPosition = XMVector3TransformCoord(localPosition, worldMatrix);

	//	XMStoreFloat3(&positionArray[i], worldPosition);
	//}
}

bool BoxCollider::DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	//return otherCollider->CollidesWithBox(this, mtvAxis, mtvDistance);
}

void BoxCollider::Move(DirectX::XMFLOAT3 moveVector, float deltaTime)
{
	//DirectX::XMFLOAT3 newDirection = FLOAT3MULT1(moveVector, deltaTime);
	//DirectX::XMFLOAT3 newPosition = FLOAT3ADD(this->transform.GetPosition(), newDirection);

	//this->SetPosition(newPosition);
}

void BoxCollider::Rotate(DirectX::XMFLOAT3 rotationVector, float deltaTime)
{
	//DirectX::XMFLOAT3 roation = FLOAT3MULT1(rotationVector, deltaTime);
	//DirectX::XMFLOAT3 newRotation = FLOAT3ADD(this->transform.GetPosition(), rotationVector);

	//this->SetRotation(newRotation);
}

void BoxCollider::SetExtents(DirectX::XMFLOAT3 extents)
{
	//this->transform.SetScale(extents);
	//this->BuildCornersArray(this->satData.positionData);

	//if (extents.x < this->shortestExtent) this->shortestExtent = extents.x;
	//if (extents.y < this->shortestExtent) this->shortestExtent = extents.y;
	//if (extents.z < this->shortestExtent) this->shortestExtent = extents.z;
}

DirectX::XMFLOAT3 BoxCollider::GetExtents()
{
	//return this->transform.GetScale();
}

void BoxCollider::SetPosition(DirectX::XMFLOAT3 newCenter)
{
	//this->transform.SetPosition(newCenter);
	//this->satData.center = newCenter;
	//this->BuildCornersArray(this->satData.positionData);
}

void BoxCollider::SetRotation(DirectX::XMFLOAT3 newRotation)
{
	//this->transform.SetRotation(newRotation);

	//DirectX::XMFLOAT3 rotation = newRotation;
	//DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
	//DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVector);
	//DirectX::XMStoreFloat3(&this->axis[0], rotationMatrix.r[0]);
	//DirectX::XMStoreFloat3(&this->axis[1], rotationMatrix.r[1]);
	//DirectX::XMStoreFloat3(&this->axis[2], rotationMatrix.r[2]);

	//this->satData.normalData = this->axis;
	//this->BuildCornersArray(this->satData.positionData);
}
