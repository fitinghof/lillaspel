#pragma once

#include <Windows.h>

#include <exception>
#include <iostream>
#include <string>
#include <windowsx.h>
#include <functional>

class Window {
private:
    HWND hWnd;
    UINT width;
    UINT height;
    HINSTANCE instance;
    RECT windowedRect{};
    bool isFullscreen;

    static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void UpdateClientSize();

	std::function<void(UINT, UINT)> resizeCallback;

public:
    Window(const HINSTANCE instance, int nCmdShow, const std::string name = "Window", const UINT width = 1024, const UINT height = 576);
    ~Window();

    HWND GetHWND() const;
    UINT GetWidth() const;
    UINT GetHeight() const;
    void Show(int nCmdShow);
	void Resize(UINT width, UINT height);
	void ToggleFullscreen(bool fullscreen);

    void SetResizeCallback(std::function<void(UINT, UINT)> callback);
};
