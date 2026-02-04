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

void GameObject3D::LoadFromJson(const nlohmann::json& data)
{
	auto position = data.at("transform").at("position");

	this->transform.SetPosition(DirectX::XMVectorSet(position[0], position[1], position[2], 1));
}

nlohmann::json GameObject3D::SaveToJson()
{
	auto thisJson = nlohmann::json
	{
		{"type", "GameObject3D"},
		{ "transform",
			{"position",
				{ this->transform.GetPosition().m128_f32[0], this->transform.GetPosition().m128_f32[2], this->transform.GetPosition().m128_f32[3]}
			}
		}
	};

	return thisJson;
}
