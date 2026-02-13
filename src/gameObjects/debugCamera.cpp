#include "gameObjects/debugCamera.h"

#include "core/input/inputManager.h"
#include "utilities/logger.h"
#include "utilities/time.h"

#include "imgui.h"

void DebugCamera::Tick() {
	this->CameraObject::Tick();
	
	if (keyboardInput.Quit()) {
		PostQuitMessage(0);
	}

	// Skip game input if ImGui is capturing mouse or keyboard
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}

	static bool showCursor = true;

	if (keyboardInput.Interact()) { // 'F'
		showCursor = !showCursor;
		ShowCursor(showCursor);
	}

	float speed = Time::GetInstance().GetDeltaTime() * 15;
	this->transform.Move(this->transform.GetDirectionVector(), keyboardInput.GetMovementVector()[1] * speed);
	this->transform.Move(DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), this->transform.GetRotationQuaternion()),
						 keyboardInput.GetMovementVector()[0] * speed);

	if (!showCursor) {
		std::array<float, 2> lookVector = keyboardInput.GetLookVector();

		static float rot[3] = {0, 0, 0};

		float sensitivity = 2.f;
		float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();

		rot[0] += rotSpeed * lookVector[1];
		rot[1] += rotSpeed * lookVector[0];

		if (rot[0] > 1.5f) rot[0] = 1.5f;
		if (rot[0] < -1.5f) rot[0] = -1.5f;

		this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);
	}

	InputManager::GetInstance().ReadControllerInput(this->controllerInput.GetControllerIndex());

	if (this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}

	InputManager::GetInstance().ReadControllerInput(this->controllerInput.GetControllerIndex());

	if (this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}
}

void DebugCamera::SaveToJson(nlohmann::json& data) 
{
	this->CameraObject::SaveToJson(data);
	data["type"] = "DebugCamera";
}
