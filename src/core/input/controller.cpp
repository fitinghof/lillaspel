#include "core/input/controller.h"

#include <Windows.h>
#include <cmath>

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE 7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD 30

#define MAX_THUMB_MAGNITUDE 32767.0f
#define MAX_TRIGGER_MAGNITUDE 255.0f // May not be needed

bool Controller::IsConnected()
{
	this->previousState = this->state;

	DWORD dwResult = XInputGetState(this->controllerIndex, &this->state);
	return (dwResult == ERROR_SUCCESS);
}

bool Controller::HasUpdatedState() const {
	if (this->previousState.dwPacketNumber == this->state.dwPacketNumber)
		return false;
	return true;
}

ControllerInput& Controller::ReadNewInput()
{
	XINPUT_GAMEPAD gamepad = this->state.Gamepad;
	this->input.buttons = gamepad.wButtons;

	// Deadzone LEFT STICK
	float LX = gamepad.sThumbLX;
	float LY = gamepad.sThumbLY;

	float leftMagnitude = sqrt(LX * LX + LY * LY);
	float normMagnitude = leftMagnitude / (MAX_THUMB_MAGNITUDE - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);

	float normLX = LX / leftMagnitude;
	float normLY = LY / leftMagnitude;

	if (leftMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		if(normMagnitude > 1.0f) 
			normMagnitude = 1.0f;

		LX = normLX * normMagnitude;
		LY = normLY * normMagnitude;
	} 
	else {
		LX = 0.0f;
		LY = 0.0f;
	}

	this->input.leftThumb[0] = LX;
	this->input.leftThumb[1] = LY;
	
	// Deadzone RIGHT STICK
	float RX = gamepad.sThumbRX;
	float RY = gamepad.sThumbRY;

	float rightMagnitude = sqrt(RX * RX + RY * RY);
	normMagnitude = rightMagnitude / (MAX_THUMB_MAGNITUDE - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);

	float normRX = RX / rightMagnitude;
	float normRY = RY / rightMagnitude;

	if (rightMagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		if (normMagnitude > 1.0f)
			normMagnitude = 1.0f;

		RX = normRX * normMagnitude;
		RY = normRY * normMagnitude;
	}
	else {
		RX = 0.0f;
		RY = 0.0f;
	}

	this->input.rightThumb[0] = RX;
	this->input.rightThumb[1] = RY;

	// Deadzone BACK TRIGGERS ( ON / OFF )
	float LT = gamepad.bLeftTrigger;
	if (LT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		LT = 1.f;
	}
	else {
		LT = 0.0f;
	}

	this->input.leftBackTrigger = bool(LT);

	float RT = gamepad.bRightTrigger;
	if (RT > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		RT = 1.f;
	}
	else {
		RT = 0.0f;
	}
	this->input.rightBackTrigger = bool(RT);

	return this->input;
}
