#include "core/input/inputManager.h"

InputManager::InputManager() : keyStates{}, controllers{ { Controller(DWORD(0)), Controller(DWORD(1)), Controller(DWORD(2)), Controller(DWORD(3)) } }, 
inputs{}, mousePosition{ 0, 0 }, mouseMovement{ 0, 0 }, LM{ 0 }, RM{ 0 } {}

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

LRESULT InputManager::ReadMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const unsigned char key = static_cast<unsigned char>(wParam);
	switch (message) {

	/// Handle keyboard events
	case WM_KEYDOWN: {
		const bool wasDown = lParam & (1 << 30);
		if (!wasDown)
			this->SetKeyState(key, KEY_DOWN | KEY_PRESSED);
		return 0;
	}

	case WM_KEYUP: {
		this->SetKeyState(key, KEY_RELEASED);
		return 0;
	}

	/// Handle mouse events
	case WM_MOUSEMOVE: {
		const int xPos = GET_X_LPARAM(lParam);
		const int yPos = GET_Y_LPARAM(lParam);
		this->SetMousePosition(xPos, yPos);
		return 0;
	}

	case WM_LBUTTONDOWN: {
		if (!this->IsLMDown())
			this->SetLMouseKeyState(KEY_DOWN | KEY_PRESSED);
		return 0;
	}

	case WM_LBUTTONUP: {
		this->SetLMouseKeyState(KEY_RELEASED);
		return 0;
	}

	case WM_RBUTTONDOWN: {
		if (!this->IsRMDown())
			this->SetRMouseKeyState(KEY_DOWN | KEY_PRESSED);
		return 0;
	}

	case WM_RBUTTONUP: {
		this->SetRMouseKeyState(KEY_RELEASED);
		return 0;
	}

	/// Handle window events
	/*case WM_SIZE: {
		if (wParam != SIZE_MINIMIZED) {
			UINT clientWidth = LOWORD(lParam);
			UINT clientHeight = HIWORD(lParam);

			if (clientWidth > 0 && clientHeight > 0) {
				if (!this->isFullscreen) {
					this->width = clientWidth;
					this->height = clientHeight;

					if (this->resizeCallback) {
						this->resizeCallback(this->width, this->height);
					}
				}
			}
		}
		return 0;
	}*/

	case WM_DESTROY: {
		PostQuitMessage(0);
		return 0;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
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
	for (DWORD i = 0; i < connectedControllers.size(); ++i) {
		this->inputs[i] = connectedControllers[i].ReadInput();
	}
}

std::array<float, 2> InputManager::GetLeftThumbMovement(DWORD index) const { return this->inputs[index].leftThumb; }

std::array<float, 2> InputManager::GetRightThumbMovement(DWORD index) const { return this->inputs[index].rightThumb; }

bool InputManager::IsLeftBackTriggerPressed(DWORD index) const { return this->inputs[index].leftBackTrigger; }

bool InputManager::IsRightBackTriggerPressed(DWORD index) const { return this->inputs[index].rightBackTrigger; }

bool InputManager::IsControllerButtonPressed(DWORD index, const ControllerInputBinMask button) const { return (this->inputs[index].buttons & button) != 0; }