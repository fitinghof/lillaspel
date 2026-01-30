#include "core/inputManager.h"

InputManager::InputManager() : keyStates{}, mousePosition{ 0, 0 }, mouseMovement{ 0, 0 }, LM{ 0 }, RM{ 0 } {}

LRESULT InputManager::ReadInput(Window* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		
		/// Handle keyboard events
		case WM_KEYDOWN:
			const unsigned char key = static_cast<unsigned char>(wParam);

			switch (key) {
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;

				case VK_F11:
					//window->SetFullscreen(!window->IsFullscreen());
					break;
				
				default:
					const bool wasDown = lParam & (1 << 30);
					if(!wasDown)
						this->SetKeyState(key, KEY_DOWN | KEY_PRESSED);
					break;
			}
			return 0;

		case WM_KEYUP:
			const unsigned char key = static_cast<unsigned char>(wParam);
			this->SetKeyState(key, KEY_RELEASED);
			return 0;

		/// Handle mouse events
		case WM_MOUSEMOVE:
			const int xPos = GET_X_LPARAM(lParam);
			const int yPos = GET_Y_LPARAM(lParam);

			return 0;

		case WM_LBUTTONDOWN:
			
			return 0;

		case WM_LBUTTONUP:
			
			return 0;

		case WM_RBUTTONDOWN:
			
			return 0;

		case WM_RBUTTONUP:
			
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

void InputManager::SetKeyState(const unsigned char key, const unsigned char state) { keyStates[key] = state; }

void InputManager::SetLMouseKeyState(const unsigned char state)
{
	this->LM = state;
}

void InputManager::SetRMouseKeyState(const unsigned char state)
{
	this->RM = state;
}

void InputManager::Reset()
{
	for (unsigned char& state : keyStates)
	{
		state &= KEY_DOWN;
		this->mouseMovement = { 0, 0 };
		this->LM &= KEY_DOWN;
		this->RM &= KEY_DOWN;
	}
}

std::pair<unsigned int, unsigned int> InputManager::GetMouseMovement() const
{
	return this->mouseMovement;
}

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
