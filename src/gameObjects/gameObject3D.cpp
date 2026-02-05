#include "gameObjects/gameObject3D.h"

GameObject3D::GameObject3D() : transform()
{
}

void GameObject3D::Tick()
{
	
}

DirectX::XMVECTOR GameObject3D::GetGlobalPosition() const
{
	return GetDecomposedWorldMatrix(TransformComponent::TRANSLATAION);
}

DirectX::XMVECTOR GameObject3D::GetGlobalRotation() const
{
	return GetDecomposedWorldMatrix(TransformComponent::ROTATION);
}

DirectX::XMVECTOR GameObject3D::GetGlobalScale() const
{
	return GetDecomposedWorldMatrix(TransformComponent::SCALE);
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

DirectX::XMVECTOR GameObject3D::GetGlobalForward() const
{
	return DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), GetGlobalRotation());
}

void GameObject3D::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject::LoadFromJson(data);

	if (data.contains("transform")) {
		if (data.contains("position")) {
			auto position = data.at("transform").at("position");

			this->transform.SetPosition(DirectX::XMVectorSet(position[0], position[1], position[2], 1));
		}
	}
}

void GameObject3D::SaveToJson(nlohmann::json& data)
{
	this->GameObject::SaveToJson(data);

	data["type"] = "GameObject3D";
	data["transform"]["position"] = { this->transform.GetPosition().m128_f32[0], this->transform.GetPosition().m128_f32[1], this->transform.GetPosition().m128_f32[2] };
}

DirectX::XMVECTOR GameObject3D::GetDecomposedWorldMatrix(const TransformComponent& component) const
{
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotationQuaternion;
	DirectX::XMVECTOR translation;

	DirectX::XMMatrixDecompose(&scale, &rotationQuaternion, &translation, GetGlobalWorldMatrix(false));
	
	switch (component) {
	case TransformComponent::SCALE:
		return scale;
	case TransformComponent::ROTATION:
		return rotationQuaternion;
	case TransformComponent::TRANSLATAION:
		return translation;
	default:
		throw std::runtime_error("Failed GameObject3D::GetDecomposedWorldMatrix()");
	}
}
