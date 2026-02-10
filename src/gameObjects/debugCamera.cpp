#include "gameObjects/debugCamera.h"

#include "core/input/inputManager.h"
#include "utilities/time.h"
#include "utilities/logger.h"

#include "imgui.h"

void DebugCamera::Tick()
{
	this->CameraObject::Tick();

	// Skip game input if ImGui is capturing mouse or keyboard
	if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) {
		return;
	}

	if (keyboardInput.Quit()) {
		PostQuitMessage(0);
	}

	if (keyboardInput.Jump()) {
		Logger::Log("Jump pressed");
	}
	
	std::array<float, 2> movementVector = keyboardInput.GetMovementVector();
	if (movementVector[0] != 0.0f || movementVector[1] != 0.0f) {
		Logger::Log("Movement vector: " + std::to_string(movementVector[0]) + ", " + std::to_string(movementVector[1]));
	}

	std::array<float, 2> lookVector = keyboardInput.GetLookVector();
	if (lookVector[0] != 0.0f || lookVector[1] != 0.0f) {
		Logger::Log("Look vector: " + std::to_string(lookVector[0]) + ", " + std::to_string(lookVector[1]));
	}


	InputManager::GetInstance().ReadControllerInput(this->controllerInput.GetControllerIndex());

	if (this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}

	if (this->controllerInput.Jump()) {
		Logger::Log("Controller Jump pressed");
	}

	if (this->controllerInput.LeftClick()) {
		Logger::Log("Controller Left Click pressed");
	}
	
	movementVector = this->controllerInput.GetMovementVector();
	if (movementVector[0] != 0.0f || movementVector[1] != 0.0f) {
		Logger::Log("Controller Movement vector: " + std::to_string(movementVector[0]) + ", " + std::to_string(movementVector[1]));
	}

	lookVector = this->controllerInput.GetLookVector();
	if (lookVector[0] != 0.0f || lookVector[1] != 0.0f) {
		Logger::Log("Controller Look vector: " + std::to_string(lookVector[0]) + ", " + std::to_string(lookVector[1]));
	}
}
