#pragma once

#include <Windows.h>
#include <array>
#include <vector>

#include "core/input/controller.h"

enum KeyStateBinMask {
	KEY_DOWN = 1,
	KEY_PRESSED = 2,
	KEY_RELEASED = 4
};

enum ControllerInputBinMask {
	DPAD_UP = 0x0001,
	DPAD_DOWN = 0x0002,
	DPAD_LEFT = 0x0004,
	DPAD_RIGHT = 0x0008,

	START = 0x0010,
	BACK = 0x0020,

	LEFT_THUMB = 0x0040,
	RIGHT_THUMB = 0x0080,
	LEFT_SHOULDER = 0x0100,
	RIGHT_SHOULDER = 0x0200,

	A_BUTTON = 0x1000,
	B_BUTTON = 0x2000,
	X_BUTTON = 0x4000,
	Y_BUTTON = 0x8000
};


class InputManager {
private:
	std::array<unsigned char, 256> keyStates;

	std::array<Controller, XUSER_MAX_COUNT> controllers;
	std::array<ControllerInput, XUSER_MAX_COUNT> inputs;

	std::vector<Controller> GetConnectedControllers();

	std::pair<unsigned int, unsigned int> mousePosition;
	std::pair<int, int> mouseMovement;
	unsigned char LM;
	unsigned char RM;

	// Keyboard
	void SetKeyState(const unsigned char key, const unsigned char state);

	// Mouse
	void SetLMouseKeyState(const unsigned char state);
	void SetRMouseKeyState(const unsigned char state);

	void SetMousePosition(const int x, const int y);

public:
	InputManager();
	~InputManager() = default;
	void Reset();

	bool ReadMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Keyboard
	bool IsKeyDown(const unsigned char key) const;
	bool WasKeyPressed(const unsigned char key) const;
	bool WasKeyReleased(const unsigned char key) const;

	// Mouse
	std::pair<unsigned int, unsigned int> GetMouseMovement() const;
	std::pair<int, int> GetMousePosition() const;

	bool IsLMDown() const;
	bool WasLMPressed() const;
	bool WasLMReleased() const;

	bool IsRMDown() const;
	bool WasRMPressed() const;
	bool WasRMReleased() const;

	// Controllers
	void ReadControllerInputs();

	std::array<float, 2> GetLeftThumbMovement(DWORD index) const;
	std::array<float, 2> GetRightThumbMovement(DWORD index) const;
	bool IsLeftBackTriggerPressed(DWORD index) const;
	bool IsRightBackTriggerPressed(DWORD index) const;
	bool IsControllerButtonPressed(DWORD index, const ControllerInputBinMask button) const;
};