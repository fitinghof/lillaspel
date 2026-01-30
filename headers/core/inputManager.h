#pragma once

#include <Windows.h>
#include <array>
#include "core/window.h"

enum KeyStateBinMask {
	KEY_DOWN = 1,
	KEY_PRESSED = 2,
	KEY_RELEASED = 4
};


class InputManager {
public:
	InputManager();
	~InputManager() = default;

	LRESULT ReadInput(Window* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetKeyState(const unsigned char key, const unsigned char state);
	void SetLMouseKeyState(const unsigned char state);
	void SetRMouseKeyState(const unsigned char state);

	void Reset();

	std::pair<unsigned int, unsigned int> GetMouseMovement() const;
	std::pair<int, int> GetMousePosition() const;

	bool IsKeyDown(const unsigned char key) const;
	bool WasKeyPressed(const unsigned char key) const;
	bool WasKeyReleased(const unsigned char key) const;

	bool IsLMDown() const;
	bool WasLMPressed() const;
	bool WasLMReleased() const;

	bool IsRMDown() const;
	bool WasRMPressed() const;
	bool WasRMReleased() const;

private:
	std::array<unsigned char, 256> keyStates;

	std::pair<unsigned int, unsigned int> mousePosition;
	std::pair<int, int> mouseMovement;
	unsigned char LM;
	unsigned char RM;
};