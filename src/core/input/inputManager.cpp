#include "core/input/inputManager.h"

InputManager::InputManager() : keyStates{}, controllers{ { Controller(DWORD(0)), Controller(DWORD(1)), Controller(DWORD(2)), Controller(DWORD(3)) } }, 
input{}, mousePosition{ 0, 0 }, mouseMovement{ 0, 0 }, LM{ 0 }, RM{ 0 } {}

void InputManager::SetKeyState(const unsigned char key, const unsigned char state) { keyStates[key] = state; }

void InputManager::SetLMouseKeyState(const unsigned char state) { this->LM = state; }

void InputManager::SetRMouseKeyState(const unsigned char state) { this->RM = state; }

void InputManager::SetMousePosition(const int x, const int y) {
	this->mouseMovement = { (int)x - this->mousePosition.first, (int)y - this->mousePosition.second };
	this->mousePosition = { x, y };
}

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;

}

void InputManager::Reset() {
	for (unsigned char& state : keyStates)
	{
		state &= KEY_DOWN;
		this->mouseMovement = { 0, 0 };
		this->LM &= KEY_DOWN;
		this->RM &= KEY_DOWN;
	}
}

std::pair<unsigned int, unsigned int> InputManager::GetMouseMovement() const { return this->mouseMovement; }

std::pair<int, int> InputManager::GetMousePosition() const { return this->mousePosition; }

bool InputManager::IsKeyDown(const unsigned char key) const { return keyStates[key] & KEY_DOWN; }

bool InputManager::WasKeyPressed(const unsigned char key) const { return keyStates[key] & KEY_PRESSED; }

bool InputManager::WasKeyReleased(const unsigned char key) const { return keyStates[key] & KEY_RELEASED; }

bool InputManager::IsLMDown() const { return this->LM & KEY_DOWN; }

bool InputManager::WasLMPressed() const { return this->LM & KEY_PRESSED; }

bool InputManager::WasLMReleased() const { return this->LM & KEY_RELEASED; }

bool InputManager::IsRMDown() const { return this->RM & KEY_DOWN; }

bool InputManager::WasRMPressed() const { return this->RM & KEY_PRESSED; }

bool InputManager::WasRMReleased() const { return this->RM & KEY_RELEASED; }

std::vector<Controller> InputManager::GetConnectedControllers() {
	std::vector<Controller> connectedControllers;
	for (auto& controller : this->controllers) {
		if (controller.IsConnected()) {
			connectedControllers.push_back(controller);
		}
	}
	return connectedControllers;
}

void InputManager::ReadControllerInputs() {
	
	std::vector<Controller> connectedControllers = this->GetConnectedControllers();
	for (auto& controller : connectedControllers) {
		if(controller.HasUpdatedState()) {
			this->input = controller.ReadNewInput();
		}
	}
}

std::array<float, 2> InputManager::GetLeftThumbMovement() const { return this->input.leftThumb;  }

std::array<float, 2> InputManager::GetRightThumbMovement() const { return this->input.rightThumb; }

bool InputManager::IsLeftBackTriggerPressed() const { return this->input.leftBackTrigger; }

bool InputManager::IsRightBackTriggerPressed() const { return this->input.rightBackTrigger; }

bool InputManager::IsControllerButtonPressed(const ControllerInputBinMask button) const { return (this->input.buttons & button) != 0; }