#include "gameObjects/pointLightObject.h"

PointLightObject::PointLightObject() {
	this->data = {};
	DirectX::XMStoreFloat3(&this->data.position, DirectX::XMVectorSet(0, 0, 0, 0));
	DirectX::XMStoreFloat4(&this->data.color, DirectX::XMVectorSet(1, 1, 1, 1));

	this->shadowViewPort = {
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = static_cast<FLOAT>(128),
		.Height = static_cast<FLOAT>(128),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f,
	};

	Logger::Log("Created a pointLight.");
	this->resolutionChanged = true;
}

PointLightObject::PointLightContainer PointLightObject::GetPointLightData() {
	DirectX::XMStoreFloat3(&this->data.position, GetGlobalPosition());

	for (size_t i = 0; i < 6; i++) {
		if (this->cameras[i].expired()) {
			Logger::Error("One of PointLight shadowcameras were expired");
			continue;
		}
		this->data.viewProjectionMatrix[i] = this->cameras[i].lock()->GetCameraMatrix(true).viewProjectionMatrix;
	}

	return this->data;
}

std::array<ID3D11DepthStencilView*, 6> PointLightObject::GetDepthStencilViews() const {
	return {
		this->shadowbuffer[0].GetDepthStencilView(), this->shadowbuffer[1].GetDepthStencilView(),
		this->shadowbuffer[2].GetDepthStencilView(), this->shadowbuffer[3].GetDepthStencilView(),
		this->shadowbuffer[4].GetDepthStencilView(), this->shadowbuffer[5].GetDepthStencilView(),
	};
}

const D3D11_VIEWPORT& PointLightObject::GetViewPort() const {
	if (this->resolutionChanged) {
		Logger::Warn("Note that shadow resolution has changed But buffer has not"
					 ", Do not use this viewport for rendering shadow before calling SetDepthBuffer");
	}
	return this->shadowViewPort;
}

bool PointLightObject::GetResolutionChanged() const { return this->resolutionChanged; }

std::array<ID3D11ShaderResourceView*, 6> PointLightObject::GetSRVs() const {
	return {
		this->shadowbuffer[0].GetShaderResourceView(), this->shadowbuffer[1].GetShaderResourceView(),
		this->shadowbuffer[2].GetShaderResourceView(), this->shadowbuffer[3].GetShaderResourceView(),
		this->shadowbuffer[4].GetShaderResourceView(), this->shadowbuffer[5].GetShaderResourceView(),
	};
}

void PointLightObject::Start() {
	RenderQueue::AddLightObject(this->GetPtr());

	// Attach camera for shadowpass
	for (size_t i = 0; i < 6; i++) {
		this->cameras[i] = this->factory->CreateGameObjectOfType<CameraObject>();
		this->cameras[i].lock()->SetAspectRatio(1 / 1);
		this->cameras[i].lock()->SetFarPlane(20.);
	}

	// Pray the directions are correct
	this->cameras[0].lock()->transform.SetRotationRPY(0, DirectX::XM_PIDIV2, 0);
	this->cameras[1].lock()->transform.SetRotationRPY(0, DirectX::XM_PIDIV2 * 2, 0);
	this->cameras[2].lock()->transform.SetRotationRPY(0, DirectX::XM_PIDIV2 * 3, 0);
	this->cameras[3].lock()->transform.SetRotationRPY(0, DirectX::XM_PIDIV2 * 4, 0);
	this->cameras[4].lock()->transform.SetRotationRPY(DirectX::XM_PIDIV2, 0, 0);
	this->cameras[5].lock()->transform.SetRotationRPY(-DirectX::XM_PIDIV2, 0, 0);
}

void PointLightObject::Tick() {

	// feels bad doing this every frame
	DirectX::XMStoreFloat3(&this->data.position, GetGlobalPosition());

	for (size_t i = 0; i < 6; i++) {
		auto& cameraWeak = this->cameras[i];
		if (cameraWeak.expired()) {
			Logger::Error("Pointligt camera was expired");
			continue;
		}
		auto cameraLocked = cameraWeak.lock();

		auto viewProj = cameraLocked->GetCameraMatrix(true).viewProjectionMatrix;

		this->data.viewProjectionMatrix[i] = viewProj;
	}
}

void PointLightObject::LoadFromJson(const nlohmann::json& data) {
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("color")) {
		auto newColor = data["color"];
		DirectX::XMStoreFloat4(&this->data.color,
							   DirectX::XMVectorSet(newColor[0], newColor[1], newColor[2], newColor[3]));
	}

	if (data.contains("intensity")) {
		this->data.intensity = data["intensity"].get<float>();
	}
}

void PointLightObject::SaveToJson(nlohmann::json& data) {
	this->GameObject3D::SaveToJson(data);

	data["type"] = "PointLightObject";

	DirectX::XMVECTOR currentColor = DirectX::XMLoadFloat4(&this->data.color);
	data["color"] = {currentColor.m128_f32[0], currentColor.m128_f32[1], currentColor.m128_f32[2],
					 currentColor.m128_f32[3]};

	data["intensity"] = this->data.intensity;
}

void PointLightObject::SetShadowResolution(size_t width, size_t height) {
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

void PointLightObject::SetDepthBuffers(ID3D11Device* device) {
	for (auto& shadowbuff : this->shadowbuffer) {
		shadowbuff.Init(device, this->shadowViewPort.Width, this->shadowViewPort.Height);
	}
	this->resolutionChanged = false;
}

void PointLightObject::ShowInHierarchy() {
	this->GameObject3D::ShowInHierarchy();

	ImGui::Text("PointLight");
	float intensity = this->data.intensity;
	ImGui::SliderFloat("Intensity", &intensity, 0.0f, 200.0f);
	this->data.intensity = intensity;
}
