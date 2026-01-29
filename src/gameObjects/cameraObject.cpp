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
	UpdateCameraMatrix();
}

void CameraObject::UpdateCameraMatrix()
{
	// Position

	this->cameraMatrix.cameraPosition = transform.GetPosition();


	// View Projection Matrix

	DirectX::XMVECTOR focusPos = DirectX::XMVectorSet(0, 0, 1, 1.0f);
	DirectX::XMVECTOR upDir = DirectX::XMVectorSet(0, 0, 1, 1.0f);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(this->cameraMatrix.cameraPosition, focusPos, upDir);

	float tempAspectRatio = 16.0f / 9.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fieldOfView), tempAspectRatio, 0.1f, 1000.0f);

	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMStoreFloat4x4(&this->cameraMatrix.viewProjectionMatrix, viewProjMatrix);
}
