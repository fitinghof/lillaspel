#pragma once

#define _AMD64_

#include <windows.h>
#include <xinput.h>
#include <array>

struct RawControllerInput {
	WORD buttons = 0;							// Bitmask of pressed buttons
	std::array<float, 2> leftThumb = { 0, 0 };	// [-1.0f, 1.0f]
	std::array<float, 2> rightThumb = { 0, 0 };	// [-1.0f, 1.0f]
	bool leftBackTrigger = false;				// ON / OFF
	bool rightBackTrigger = false;				// ON / OFF
};

class Controller {
public:
	Controller(DWORD controllerIndex) : controllerIndex(controllerIndex), state{}, previousState{}, input{} {}
	~Controller() = default;

	const bool IsConnected();
	void ReadInput();

	const RawControllerInput& GetInput() const;

	void PrintInputState();

private:
	DWORD controllerIndex;
	XINPUT_STATE state;
	XINPUT_STATE previousState;

	RawControllerInput input;
};