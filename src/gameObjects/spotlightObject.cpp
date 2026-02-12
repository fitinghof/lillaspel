#include "gameObjects/spotlightObject.h"
#include "gameObjects/cameraObject.h"

SpotlightObject::SpotlightObject() {
	this->data = {};
	DirectX::XMStoreFloat3(&this->data.position, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat3(&this->data.direction, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(1, 1, 1, 1));

	this->data.intensity = 5;
	this->data.spotCosAngleRadians = cos(DirectX::XMConvertToRadians(120) / 2);

	this->shadowViewPort = {
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<FLOAT>(128),
		.Height = static_cast<FLOAT>(128),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

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
	// feels bad doing this every frame
	DirectX::XMStoreFloat3(&this->data.position, GetGlobalPosition());
	DirectX::XMStoreFloat3(&this->data.direction, GetGlobalForward());

	auto projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(this->data.spotCosAngleRadians, 1, 1, 100);
	auto viewMatrix = this->GetGlobalViewMatrix();

	DirectX::XMStoreFloat4x4(&this->data.viewProjectionMatrix,
							 DirectX::XMMatrixMultiplyTranspose(viewMatrix, projectionMatrix));
}

void SpotlightObject::LoadFromJson(const nlohmann::json& data) {
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("color")) {
		auto newColor = data["color"];
		DirectX::XMStoreFloat4(&this->data.color,
							   DirectX::XMVectorSet(newColor[0], newColor[1], newColor[2], newColor[3]));
	}

	if (data.contains("intensity")) {
		this->data.intensity = data["intensity"].get<float>();
	}

	if (data.contains("angleRadians")) {
		this->data.spotCosAngleRadians = data["angleRadians"].get<float>();
	}
}

void SpotlightObject::SaveToJson(nlohmann::json& data) {
	this->GameObject3D::SaveToJson(data);

	data["type"] = "SpotlightObject";

	DirectX::XMVECTOR currentColor = DirectX::XMLoadFloat4(&this->data.color);
	data["color"] = {currentColor.m128_f32[0], currentColor.m128_f32[1], currentColor.m128_f32[2],
					 currentColor.m128_f32[3]};

	data["intensity"] = this->data.intensity;

	data["angleRadians"] = this->data.spotCosAngleRadians;
}

const D3D11_VIEWPORT& SpotlightObject::GetViewPort() const {
	if (this->resolutionChanged) {
		Logger::Warn("Note that shadow resolution has changed But buffer has not"
					 ", Do not use this viewport for rendering shadow before calling SetDepthBuffer");
	}
	return this->shadowViewPort;
}

bool SpotlightObject::GetResolutionChanged() const { return this->resolutionChanged; }

ID3D11ShaderResourceView* SpotlightObject::GetSRV() const { return this->shadowbuffer.GetShaderResourceView(); }

void SpotlightObject::SetShadowResolution(size_t width, size_t height) {
	this->shadowViewPort = {
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<FLOAT>(width),
		.Height = static_cast<FLOAT>(height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

	this->resolutionChanged = true;
}

void SpotlightObject::SetDepthBuffer(ID3D11Device* device) {
	this->shadowbuffer.Init(device, this->shadowViewPort.Width, this->shadowViewPort.Height);
	this->resolutionChanged = false;
}
