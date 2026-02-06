#include "gameObjects/cameraObject.h"
#include "DirectXMath.h"

CameraObject* CameraObject::mainCamera = nullptr;
size_t CameraObject::cameraId = 0;

CameraObject::CameraObject() : fieldOfView(80.0f), thisCameraId(CameraObject::cameraId++)
{
	if (!this->mainCamera) {
		this->mainCamera = this;
	}

	UpdateCameraMatrix();

	Logger::Log("Successfully created a camera.");
}

void CameraObject::Tick()
{
	if (this->thisCameraId != CameraObject::mainCamera->thisCameraId) {
		ImGui::SetNextWindowSize(ImVec2(150, 120.f));
		ImGui::Begin("MainCamera");
		ImGui::Text("Switch camera");
		std::string buttonText = "Camera " + std::to_string(this->thisCameraId);
		if (ImGui::Button(buttonText.c_str()))
		{
			CameraObject::mainCamera = this;
		}
		ImGui::End();
	}

	// Until we get input

	//static float pos[3] = { 0,0,0 };
	//static float rot[3] = { 0,0,0 };

	
	InputManager::GetInstance().ReadControllerInputs();
	std::array leftStick = InputManager::GetInstance().GetLeftThumbMovement();
	std::array rightStick = InputManager::GetInstance().GetRightThumbMovement();

	float sensitivity = 2.0f; 
	float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();

	rot[0] += rightStick.at(1) * rotSpeed * -1.0f;
	rot[1] += rightStick.at(0) * rotSpeed;

	if (rot[0] > 1.5f) rot[0] = 1.5f;
	if (rot[0] < -1.5f) rot[0] = -1.5f;

	
	this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);

	this->transform.Move(this->transform.GetDirectionVector(), Time::GetInstance().GetDeltaTime() * leftStick.at(1) * 20);

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

void CameraObject::LoadFromJson(const nlohmann::json& data)
{
	this->GameObject3D::LoadFromJson(data);

	if (data.contains("fov")) {
		this->fieldOfView = data.at("fov").get<float>();
	}
}

void CameraObject::SaveToJson(nlohmann::json& data)
{
	this->GameObject3D::SaveToJson(data);

	data["type"] = "CameraObject";
	data["fov"] = this->fieldOfView;
}

void CameraObject::UpdateCameraMatrix()
{
	// Position

	this->cameraMatrix.cameraPosition = GetGlobalPosition();


	// View Projection Matrix

	DirectX::XMVECTOR globalRotation = GetGlobalRotation();

	DirectX::XMVECTOR focusPos = DirectX::XMVectorAdd(this->cameraMatrix.cameraPosition, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), globalRotation));
	DirectX::XMVECTOR upDir = DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), globalRotation);
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(this->cameraMatrix.cameraPosition, focusPos, upDir);

	float tempAspectRatio = 16.0f / 9.0f;
	DirectX::XMMATRIX projMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fieldOfView), tempAspectRatio, 0.1f, 1000.0f);


	DirectX::XMMATRIX viewProjMatrix = viewMatrix * projMatrix;

	DirectX::XMStoreFloat4x4(&this->cameraMatrix.viewProjectionMatrix, viewProjMatrix);
}