#include "gameObjects/spotlightObject.h"
#include "gameObjects/cameraObject.h"

SpotlightObject::SpotlightObject()
{
	this->data = {};
	DirectX::XMStoreFloat3(&this->data.position, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat3(&this->data.direction, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(1, 1, 1, 1));
	
	this->data.intensity = 5;
	this->data.spotAngleRadians = DirectX::XMConvertToRadians(360);

	Logger::Log("Created a spotlight.");
}

SpotlightObject::SpotLightContainer SpotlightObject::GetSpotLightData() const { return this->data; }

ID3D11DepthStencilView* SpotlightObject::GetDepthStencilView() const {
	return this->shadowbuffer.GetDepthStencilView(0);
}

void SpotlightObject::Start() { 
	RenderQueue::AddLightObject(this->GetPtr()); 

	// Attach camera for shadowpass
	this->camera = this->factory->CreateGameObjectOfType<CameraObject>();
	this->camera.lock()->SetParent(this->GetPtr());
}

void SpotlightObject::Tick() {
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

void SpotlightObject::SetShadowResolution(ID3D11Device* device, size_t width, size_t height) {
	this->shadowViewPort = {
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<FLOAT>(width),
		.Height = static_cast<FLOAT>(height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

	this->shadowbuffer.Init(device, width, height);
}
