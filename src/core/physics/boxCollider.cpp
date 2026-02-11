#include "core/physics/boxCollider.h"
#include "core/physics/sphereCollider.h"

BoxCollider::BoxCollider()
{
	this->type = ColliderType::BOX;
	this->satData.positionData = new DirectX::XMFLOAT3[8];

	this->satData.center = DirectX::XMFLOAT3(0, 0, 0);
	this->satData.nrOfPositions = 8;
	this->satData.nrOfNormals = 3;
	this->satData.normalData = this->axis;

	this->BuildCornersArray(this->satData.positionData);
}

BoxCollider::~BoxCollider()
{
	delete[] this->satData.positionData;
}

void BoxCollider::Tick()
{
	this->Collider::Tick();

	this->BuildCornersArray(this->satData.positionData);
	DirectX::XMStoreFloat3(&this->satData.center, this->GetGlobalPosition());
}

void BoxCollider::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject3D::LoadFromJson(data);

	DirectX::XMStoreFloat3(&this->satData.center, this->GetGlobalPosition()); //check positions later
	this->BuildCornersArray(this->satData.positionData);
}

bool BoxCollider::CollidesWithBox(class BoxCollider* box, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	Logger::Log(":::::::::::::::::Entered boxVSbox and sat-check::::::::::::::::");

	bool collision = SAT(this->satData, box->satData, resolveAxis, resolveDistance);

	Logger::Log(":::::::::::::::::After sat-check::::::::::::::::");

	std::string o = "resolveAxis: " + std::to_string(resolveAxis.x) + ", " + std::to_string(resolveAxis.y) + ", " + std::to_string(resolveAxis.z);
	Logger::Log(o);

	std::string b = "resolveDistance: " + std::to_string(resolveDistance);
	Logger::Log(b);

	std::string a = "this->satData center: " + std::to_string(this->satData.center.x) + ", " + std::to_string(this->satData.center.y) + ", " + std::to_string(this->satData.center.z);
	Logger::Log(a);

	std::string k = "other->satData center: " + std::to_string(box->satData.center.x) + ", " + std::to_string(box->satData.center.y) + ", " + std::to_string(box->satData.center.z);
	Logger::Log(k);

	return collision;
}

bool BoxCollider::CollidesWithSphere(SphereCollider* sphere, DirectX::XMFLOAT3& resolveAxis, float& resolveDistance)
{
	return this->BoxSphereCollision(this, sphere, resolveAxis, resolveDistance);
}

void BoxCollider::BuildCornersArray(DirectX::XMFLOAT3*& positionArray)
{
	using namespace DirectX;

	XMMATRIX worldMatrix = this->GetGlobalWorldMatrix(false); //iverse transpose?
	//worldMatrix = XMMatrixTranspose(worldMatrix);

	for (int i = 0; i < 8; i++)
	{
		XMVECTOR localPosition = XMLoadFloat3(&localBoxCorners[i]);
		XMVECTOR worldPosition = XMVector3TransformCoord(localPosition, worldMatrix);

		XMStoreFloat3(&positionArray[i], worldPosition);
	}
}

bool BoxCollider::DoubleDispatchCollision(Collider* otherCollider, DirectX::XMFLOAT3& mtvAxis, float& mtvDistance)
{
	return otherCollider->CollidesWithBox(this, mtvAxis, mtvDistance);
}

void BoxCollider::SetExtents(DirectX::XMFLOAT3 extents)
{
	this->transform.SetScale(DirectX::XMLoadFloat3(&extents));
	this->BuildCornersArray(this->satData.positionData);

	if (extents.x < this->shortestExtent) this->shortestExtent = extents.x;
	if (extents.y < this->shortestExtent) this->shortestExtent = extents.y;
	if (extents.z < this->shortestExtent) this->shortestExtent = extents.z;

	Logger::Log("----------SAT-DATA---------");

	for (int i = 0; i < 8; i++)
	{
		Logger::Log("positions: " + std::to_string(this->satData.positionData[i].x), +", " + std::to_string(this->satData.positionData[i].y) + std::to_string(this->satData.positionData[i].z));
	}

	Logger::Log("----------||---------");
}

DirectX::XMFLOAT3 BoxCollider::GetExtents()
{
	DirectX::XMFLOAT3 extents = {};
	DirectX::XMStoreFloat3(&extents, this->transform.GetScale());
	
	return extents;
}

void BoxCollider::SetPosition(DirectX::XMFLOAT3 newCenter)
{
	this->transform.SetPosition(DirectX::XMLoadFloat3(&newCenter));
	this->satData.center = newCenter;
	this->BuildCornersArray(this->satData.positionData);
}

void BoxCollider::SetRotation(DirectX::XMFLOAT3 newRotation)
{
	this->transform.SetRotationQuaternion(DirectX::XMLoadFloat3(&newRotation)); //quaternion?

	DirectX::XMFLOAT3 rotation = newRotation;
	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat3(&rotation);
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotationVector);
	DirectX::XMStoreFloat3(&this->axis[0], rotationMatrix.r[0]);
	DirectX::XMStoreFloat3(&this->axis[1], rotationMatrix.r[1]);
	DirectX::XMStoreFloat3(&this->axis[2], rotationMatrix.r[2]);

	this->satData.normalData = this->axis;
	this->BuildCornersArray(this->satData.positionData);
}
