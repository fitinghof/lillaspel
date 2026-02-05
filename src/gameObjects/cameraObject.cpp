#include "gameObjects/cameraObject.h"
#include "DirectXMath.h"

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

	ImGui::SetNextWindowSize(ImVec2(450.f, 120.f));
	ImGui::Begin("Camera");
	ImGui::PushItemWidth(350);
	//ImGui::SliderFloat3("Position", pos, -20, 20);
	//ImGui::SliderFloat3("Rotation", rot, -180, 180);
	ImGui::SliderFloat("FOV", &this->fieldOfView, 1, 180);
	ImGui::PopItemWidth();
	ImGui::End();
	InputManager::GetInstance().ReadControllerInputs();
	std::array leftStick = InputManager::GetInstance().GetLeftThumbMovement();
	std::array rightStick = InputManager::GetInstance().GetRightThumbMovement();

	Logger::Log(leftStick.at(0));
	float rotSpeed = 1 * Time::GetInstance().GetDeltaTime();
	float moveSpeed = 20 * Time::GetInstance().GetDeltaTime();
	/*rot[1] = rot[1] + mouseInput.first / 10000;
	rot[2] = rot[2] + mouseInput.second / 10000;*/
	this->transform.Rotate(-1 * rot[0], 0, 0);
	this->transform.Rotate(0, rightStick.at(0) * rotSpeed);
	this->transform.Rotate(rot[0], 0);
	this->transform.Rotate(rightStick.at(1) * rotSpeed * -1, 0);
	rot[0] = rot[0] + rightStick.at(1) * rotSpeed * -1;

	this->transform.Move(this->transform.GetDirectionVector(), moveSpeed * leftStick.at(1));

	//this->transform.SetPosition(DirectX::XMVectorSet(pos[0], pos[1], pos[2], 1.0f));
	// this->transform.SetRotationRPY(DirectX::XMConvertToRadians(rot[0]), DirectX::XMConvertToRadians(rot[1]), DirectX::XMConvertToRadians(rot[2]));

	UpdateCameraMatrix();
}

CameraObject::CameraMatrixContainer& CameraObject::GetCameraMatrix()
{
	return this->cameraMatrix;
}

CameraObject& CameraObject::GetMainCamera()
{
	if (!CameraObject::mainCamera)
	{
		throw std::runtime_error("No active camera!");
	}

	return *CameraObject::mainCamera;
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