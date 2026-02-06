#pragma once

#include "core/input/inputManager.h"
#include "core/window.h"

class ControlsHandler
{
public:
	ControlsHandler(ControlsHandler& other) = delete;
	ControlsHandler& operator=(const ControlsHandler&) = delete;

	static ControlsHandler& GetInstance();

private:
	InputManager* inputManager;
	Window* window;

	ControlsHandler();
};