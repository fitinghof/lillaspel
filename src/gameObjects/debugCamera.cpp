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

	// Until we get input
	int centerX = 1000;
	int centerY = 500;


	// Skip game input if ImGui is capturing mouse or keyboard
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}

	static bool mouseInput = false;

	if (keyboardInput.Jump()) {
		mouseInput = !mouseInput;
	}

	float speed = Time::GetInstance().GetDeltaTime() * 15;
	this->transform.Move(this->transform.GetDirectionVector(), keyboardInput.GetMovementVector()[1] * speed);
	this->transform.Move(DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), this->transform.GetRotationQuaternion()),
						 keyboardInput.GetMovementVector()[0] * speed);

	if (mouseInput) {
		POINT cursorPos;

		GetCursorPos(&cursorPos);

		static float rot[3] = {0, 0, 0};

		float sensitivity = 2.0f;
		float rotSpeed = sensitivity * Time::GetInstance().GetDeltaTime();

		rot[0] += rotSpeed * ((cursorPos.y - centerY));
		rot[1] += rotSpeed * ((cursorPos.x - centerX));

		if (rot[0] > 1.5f) rot[0] = 1.5f;
		if (rot[0] < -1.5f) rot[0] = -1.5f;

		this->transform.SetRotationRPY(0.0f, rot[0], rot[1]);

		SetCursorPos(1000, 500);
	}

	InputManager::GetInstance().ReadControllerInput(this->controllerInput.GetControllerIndex());

	if (this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}


}