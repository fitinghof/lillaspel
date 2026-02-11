#pragma once

#include <Windows.h>
#include <array>
#include <vector>
#include <type_traits>

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
public:
	InputManager(InputManager& other) = delete;
	InputManager& operator=(const InputManager&) = delete;

	static InputManager& GetInstance() {
		static InputManager instance;
		return instance;
	}

	~InputManager() = default;
	void Reset();

	bool ReadMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Keyboard
	bool IsKeyDown(const unsigned char key) const;
	bool WasKeyPressed(const unsigned char key) const;
	bool WasKeyReleased(const unsigned char key) const;

	// Mouse
	std::array<int, 2> GetMouseMovement() const;
	std::array<int, 2> GetMousePosition() const;

	bool IsLMDown() const;
	bool WasLMPressed() const;
	bool WasLMReleased() const;

	bool IsRMDown() const;
	bool WasRMPressed() const;
	bool WasRMReleased() const;

	// Controllers
	const size_t GetConnectedControllerCount();
	void ReadControllerInputs();
	void ReadControllerInput(DWORD index);

	std::array<float, 2> GetLeftThumbMovement(DWORD index) const;
	std::array<float, 2> GetRightThumbMovement(DWORD index) const;
	bool IsLeftBackTriggerDown(DWORD index) const;
	bool IsRightBackTriggerDown(DWORD index) const;
	bool WasLeftBackTriggerPressed(DWORD index) const;
	bool WasRightBackTriggerPressed(DWORD index) const;
	bool IsControllerButtonDown(DWORD index, const int button) const;
	bool WasControllerButtonPressed(DWORD index, const int button) const;
	bool WasControllerButtonReleased(DWORD index, const int button) const;
private:
	InputManager();

	std::array<unsigned char, 256> keyStates;

	std::array<Controller, XUSER_MAX_COUNT> controllers;

	std::vector<Controller> GetConnectedControllers();

	std::array<unsigned int, 2> mousePosition;
	std::array<int, 2> mouseMovement;
	unsigned char LM;
	unsigned char RM;

	HWND currentWindowHandle;

	// Keyboard
	void SetKeyState(const unsigned char key, const unsigned char state);

	// Mouse
	void SetLMouseKeyState(const unsigned char state);
	void SetRMouseKeyState(const unsigned char state);

	void SetMousePosition(const int x, const int y, bool reCenter = false);
};