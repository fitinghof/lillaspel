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

	InputManager::GetInstance().ReadControllerInput(this->controllerInput.GetControllerIndex());

	if (this->controllerInput.Quit()) {
		PostQuitMessage(0);
	}
}
