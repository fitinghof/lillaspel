#pragma once

#include <Windows.h>

#include <exception>
#include <iostream>
#include <string>
#include <windowsx.h>
#include <memory>

#include "core/inputManager.h"

class Window {
private:
    HWND hWnd;
    UINT width;
    UINT height;
    HINSTANCE instance;
    bool isFullscreen;
    bool showIMGui;
	bool cursorVisible;

	std::unique_ptr<InputManager> inputManager;

    static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT ReadMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    Window(const HINSTANCE instance, int nCmdShow, const std::string name = "Window", const UINT width = 1024, const UINT height = 576);
    ~Window();


    HWND GetHWND() const;
    UINT GetWidth() const;
    UINT GetHeight() const;
    InputManager* GetInputManager() const;
    bool IsFullscreen() const;
    void Show(int nCmdShow) const;

    bool IsIMGuiShown() const;
    void SetIMGuiShown(const bool show);
};
