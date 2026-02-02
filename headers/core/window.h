#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <exception>
#include <iostream>
#include <string>
#include <windowsx.h>

class Window {
private:
    HWND hWnd;
    UINT width;
    UINT height;
    HINSTANCE instance;
    bool isFullscreen;

    static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    Window(const HINSTANCE instance, int nCmdShow, const std::string name = "Window", const UINT width = 1024, const UINT height = 576);
    ~Window();

    HWND GetHWND() const;
    UINT GetWidth() const;
    UINT GetHeight() const;
    void Show(int nCmdShow) const;
};
