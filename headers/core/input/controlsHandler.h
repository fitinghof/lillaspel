#pragma once

#include "core/input/inputManager.h"
#include "core/window.h"

enum Keybinds {
	MOVE_FORWARD = 'W',
	MOVE_BACKWARD = 'S',
	MOVE_LEFT = 'A',
	MOVE_RIGHT = 'D',
	JUMP = VK_SPACE,
	SPRINT = VK_LSHIFT,

	LEFT_CLICK = VK_LBUTTON,
	RIGHT_CLICK = VK_RBUTTON,

	INTERACT = 'F',

	FULLSCREEN_TOGGLE = VK_F11,
	QUIT = VK_ESCAPE,
};

class ControlsHandler
{
public:
	ControlsHandler(const ControlsHandler& other) = delete;
	ControlsHandler& operator=(const ControlsHandler&) = delete;

	static ControlsHandler& GetInstance(Window* window);

private:
	ControlsHandler(Window* window);

	Window* window = nullptr;
	InputManager* inputManager = nullptr;

	void HandleKeyboardInput();
};