#pragma once

#define _AMD64_

#include <windows.h>
#include <xinput.h>
#include <array>

typedef struct {
	WORD buttons;					 // Bitmask of pressed buttons
	std::array<float, 2> leftThumb;	 // [-1.0f, 1.0f]
	std::array<float, 2> rightThumb; // [-1.0f, 1.0f]
	bool leftBackTrigger;			 // ON / OFF
	bool rightBackTrigger;			 // ON / OFF
} ControllerInput;

class Controller {
public:
	Controller(DWORD controllerIndex) : controllerIndex(controllerIndex), state{}, previousState{}, input{} {}
	~Controller() = default;

	const bool IsConnected();
	ControllerInput& ReadInput();

	void PrintInputState();

private:
	DWORD controllerIndex;
	XINPUT_STATE state;
	XINPUT_STATE previousState;

	ControllerInput input;
};