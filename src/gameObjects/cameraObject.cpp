#include "gameObjects/cameraObject.h"
#include "DirectXMath.h"

CameraObject* CameraObject::mainCamera = nullptr;
size_t CameraObject::cameraId = 0;

CameraObject::CameraObject() : fieldOfView(80.0f), thisCameraId(CameraObject::cameraId++) {
	if (!this->mainCamera) {
		SetMainCamera();
	}

	UpdateCameraMatrix();

	Logger::Log("Successfully created a camera.");
}

void CameraObject::Tick() {
	if (this->thisCameraId != CameraObject::mainCamera->thisCameraId) {
		ImGui::SetNextWindowSize(ImVec2(150, 120.f));
		ImGui::Begin("MainCamera");
		ImGui::Text("Switch camera");
		std::string buttonText = "Camera " + std::to_string(this->thisCameraId);
		if (ImGui::Button(buttonText.c_str())) {
			CameraObject::mainCamera = this;
		}
		ImGui::End();
	}
}

void CameraObject::LateTick() { UpdateCameraMatrix(); }

void CameraObject::Start() { }

CameraObject::CameraMatrixContainer& CameraObject::GetCameraMatrix() { return this->cameraMatrix; }

void CameraObject::SetMainCamera() { this->mainCamera = this; }

CameraObject& CameraObject::GetMainCamera() {
	if (!CameraObject::mainCamera) {
		throw std::runtime_error("No active camera!");
	}

	return *CameraObject::mainCamera;
}

void CameraObject::LoadFromJson(const nlohmann::json& data) {
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("fov")) {
		this->fieldOfView = data.at("fov").get<float>();
	}
}

void CameraObject::SaveToJson(nlohmann::json& data) {
	this->GameObject3D::SaveToJson(data);

	data["type"] = "CameraObject";
	data["fov"] = this->fieldOfView;
}

void CameraObject::UpdateCameraMatrix() {
	// Position

	this->cameraMatrix.cameraPosition = GetGlobalPosition();

	// View Projection Matrix

	DirectX::XMVECTOR globalRotation = GetGlobalRotation();

	DirectX::XMVECTOR focusPos = DirectX::XMVectorAdd(
		this->cameraMatrix.cameraPosition, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), globalRotation));
	DirectX::XMVECTOR upDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), globalRotation);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(this->cameraMatrix.cameraPosition, focusPos, upDir);

	float tempAspectRatio = 16.0f / 9.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fieldOfView),
																	 tempAspectRatio, 0.1f, 100.0f);

	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMStoreFloat4x4(&this->cameraMatrix.viewProjectionMatrix, viewProjMatrix);
}