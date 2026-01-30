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
private:
	std::array<unsigned char, 256> keyStates;

	std::pair<unsigned int, unsigned int> mousePosition;

public:
	InputManager() = default;
	~InputManager() = default;

	LRESULT ReadInput(Window* window, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetKeyState(const unsigned char key, const unsigned char state);
	void setMousePosition(const unsigned int x, const unsigned int y);

	bool IsKeyDown(const unsigned char key) const;
	bool WasKeyPressed(const unsigned char key) const;
	bool WasKeyReleased(const unsigned char key) const;

	bool IsLMDown() const;
	bool WasLMPressed() const;
	bool WasLMReleased() const;

	bool IsRMDown() const;
	bool WasRMPressed() const;
	bool WasRMReleased() const;
};