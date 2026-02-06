#include "core/input/controlsHandler.h"

ControlsHandler::ControlsHandler(Window* window) {
	this->window = window;
	this->inputManager = window->GetInputManager();
}

void ControlsHandler::HandleKeyboardInput()
{
	if (this->inputManager->IsKeyDown(MOVE_FORWARD)) {
		// Handle move forward
	}
	if (this->inputManager->IsKeyDown(MOVE_BACKWARD)) {
		// Handle move backward
	}
	if (this->inputManager->IsKeyDown(MOVE_LEFT)) {
		// Handle move left
	}
	if (this->inputManager->IsKeyDown(MOVE_RIGHT)) {
		// Handle move right
	}
	if (this->inputManager->IsKeyDown(JUMP)) {
		// Handle jump
	}
	if (this->inputManager->IsKeyDown(SPRINT)) {
		// Handle sprint
	}

	if (this->inputManager->WasKeyPressed(INTERACT)) {
		// Handle interact
	}

	if (this->inputManager->WasKeyPressed(FULLSCREEN_TOGGLE)) {
		this->window->ToggleFullscreen(!this->window->IsFullscreen());
	}

	if (this->inputManager->WasKeyPressed(QUIT)) {
		PostQuitMessage(0);
	}
}

ControlsHandler& ControlsHandler::GetInstance(Window* window)
{
		static ControlsHandler instance(window);
		return instance;
}