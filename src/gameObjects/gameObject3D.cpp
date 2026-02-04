#include "gameObjects/gameObject3D.h"

GameObject3D::GameObject3D() : transform()
{
}

void GameObject3D::Tick()
{
	
}

DirectX::XMVECTOR GameObject3D::GetGlobalPosition() const
{
	if (this->GetParent().expired()) {
		return this->transform.GetPosition();
	}
	else {
		return DirectX::XMVectorAdd(this->GetParent().lock()->GetGlobalPosition(), this->transform.GetPosition());
	}
}

DirectX::XMMATRIX GameObject3D::GetGlobalWorldMatrix(bool inverseTranspose) const
{
	DirectX::XMFLOAT4X4 localWorldXMFLOAT4X4 = this->transform.GetWorldMatrix(inverseTranspose);
	DirectX::XMMATRIX localWorldMatrix = DirectX::XMLoadFloat4x4(&localWorldXMFLOAT4X4);

	if (this->GetParent().expired()) {
		return localWorldMatrix;
	}
	else {
		return localWorldMatrix * this->GetParent().lock()->GetGlobalWorldMatrix(inverseTranspose);
	}
}