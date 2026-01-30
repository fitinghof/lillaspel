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
	static float xPos = 0.0f;
	static float yPos = 0.0f;
	static float zPos = 0.0f;

	ImGui::Begin("Camera Position");
	ImGui::SliderFloat("X", &xPos, -10.0f, 10.0f);
	ImGui::SliderFloat("Y", &yPos, -10.0f, 10.0f);
	ImGui::SliderFloat("Z", &zPos, -10.0f, 10.0f);
	ImGui::End();

	transform.SetPosition(DirectX::XMVectorSet(xPos, yPos, zPos, 1.0f));

	UpdateCameraMatrix();
}

CameraObject::CameraMatrixContainer& CameraObject::GetCameraMatrix()
{
	return this->cameraMatrix;
}

void CameraObject::UpdateCameraMatrix()
{
	// Position

	this->cameraMatrix.cameraPosition = transform.GetPosition();


	// View Projection Matrix

	DirectX::XMVECTOR focusPos = DirectX::XMVectorAdd(transform.GetPosition(), transform.GetDirectionVector());
	DirectX::XMVECTOR upDir = DirectX::XMVectorSet(0, 1, 0, 1.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(transform.GetPosition(), focusPos, upDir);

	float tempAspectRatio = 16.0f / 9.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fieldOfView), tempAspectRatio, 0.1f, 1000.0f);


	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMStoreFloat4x4(&this->cameraMatrix.viewProjectionMatrix, viewProjMatrix);
}
