#include "gameObjects/spotlightObject.h"

SpotlightObject::SpotlightObject()
{
	this->data = {};
	DirectX::XMStoreFloat3(&this->data.position, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat3(&this->data.direction, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(1, 1, 1, 1));
	this->data.intensity = 5;
	this->data.spotAngleRadians = 2 * DirectX::XMConvertToRadians(360);

	RenderQueue::AddLightObject(this);

	Logger::Log("Created a spotlight.");
}

void SpotlightObject::Tick()
{
	DirectX::XMStoreFloat3(&this->data.position, GetGlobalPosition());
	DirectX::XMStoreFloat3(&this->data.direction, GetGlobalForward());
}

void SpotlightObject::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("color")) {
		auto newColor = data["color"];
		DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(newColor[0], newColor[1], newColor[2], newColor[3]));
	}

	if (data.contains("intensity")) {
		this->data.intensity = data["intensity"].get<float>();
	}

	if (data.contains("angleRadians")) {
		this->data.spotAngleRadians = data["angleRadians"].get<float>();
	}
}

void SpotlightObject::SaveToJson(nlohmann::json& data)
{
	this->GameObject3D::SaveToJson(data);

	data["type"] = "SpotlightObject";

	DirectX::XMVECTOR currentColor = DirectX::XMLoadFloat4(&this->data.color);
	data["color"] = { currentColor.m128_f32[0], currentColor.m128_f32[1], currentColor.m128_f32[2], currentColor.m128_f32[3] };

	data["intensity"] = this->data.intensity;

	data["angleRadians"] = this->data.spotAngleRadians;
}
