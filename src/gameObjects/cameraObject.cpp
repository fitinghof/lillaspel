#include "gameObjects/cameraObject.h"

CameraObject* CameraObject::mainCamera = nullptr;

CameraObject::CameraObject() : fieldOfView(80.0f)
{
	if (!mainCamera) {
		mainCamera = this;
	}

	UpdateCameraMatrix();

	Logger::Log("Successfully created a camera.");
}

void CameraObject::Tick()
{
	// Until we get input

	static float pos[3] = { 0,0,0 };
	static float rot[3] = { 0,0,0 };

	ImGui::Begin("Camera");
	ImGui::PushItemWidth(350);
	ImGui::SliderFloat3("Position", pos, -20, 20);
	ImGui::SliderFloat3("Rotation", rot, -180, 180);
	ImGui::SliderFloat("FOV", &this->fieldOfView, 1, 180);
	ImGui::PopItemWidth();
	ImGui::End();

	this->transform.SetPosition(DirectX::XMVectorSet(pos[0], pos[1], pos[2], 1.0f));
	this->transform.SetRotationRPY(DirectX::XMConvertToRadians(rot[0]), DirectX::XMConvertToRadians(rot[1]), DirectX::XMConvertToRadians(rot[2]));

	UpdateCameraMatrix();
}

CameraObject::CameraMatrixContainer& CameraObject::GetCameraMatrix()
{
	return this->cameraMatrix;
}

void CameraObject::UpdateCameraMatrix()
{
	// Position

	this->cameraMatrix.cameraPosition = this->transform.GetPosition();


	// View Projection Matrix

	DirectX::XMVECTOR focusPos = DirectX::XMVectorAdd(this->transform.GetPosition(), this->transform.GetDirectionVector());
	DirectX::XMVECTOR upDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), this->transform.GetRotationQuaternion());
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(this->transform.GetPosition(), focusPos, upDir);

	float tempAspectRatio = 16.0f / 9.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fieldOfView), tempAspectRatio, 0.1f, 1000.0f);


	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMStoreFloat4x4(&this->cameraMatrix.viewProjectionMatrix, viewProjMatrix);
}