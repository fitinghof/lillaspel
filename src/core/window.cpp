#include "core/window.h"
#include "imgui.h"

LRESULT Window::StaticWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* window = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->hWnd = hWnd;
        return window->WindowProc(hWnd, message, wParam, lParam);
    }

    Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (window) {
        return window->WindowProc(hWnd, message, wParam, lParam);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
   
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;
    
    switch (message) {
    case WM_SIZE: {
        if (wParam != SIZE_MINIMIZED) {
            UINT clientWidth = LOWORD(lParam);
            UINT clientHeight = HIWORD(lParam);

            if (clientWidth > 0 && clientHeight > 0) {
                if(!this->isFullscreen) {
                    this->width = clientWidth;
                    this->height = clientHeight;

                    if (this->resizeCallback) {
                        this->resizeCallback(this->width, this->height);
                    }
				}
            }
        }
        return 0;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

void Window::UpdateClientSize()
{
    RECT rc{};
    if (GetClientRect(this->hWnd, &rc)) {
        UINT clientWidth = static_cast<UINT>(rc.right - rc.left);
        UINT clientHeight = static_cast<UINT>(rc.bottom - rc.top);
        if (clientWidth > 0 && clientHeight > 0) {
            this->width = clientWidth;
            this->height = clientHeight;
        }
    }
}

Window::Window(const HINSTANCE instance, int nCmdShow, const std::string name, const UINT width, const UINT height)
    : instance(instance), width(width), height(height), hWnd(nullptr), isFullscreen(false) {

    const wchar_t CLASS_NAME[] = L"WINDOW_CLASS";

    WNDCLASSEX wc = { .cbSize = sizeof(WNDCLASSEX),
                     .lpfnWndProc = Window::StaticWindowProc,
                     .hInstance = this->instance,
                     .lpszClassName = CLASS_NAME };

    if (!RegisterClassEx(&wc)) {
        throw std::runtime_error("Failed to register window class, error: " + std::to_string(GetLastError()));
    }

    std::wstring widestr = std::wstring(name.begin(), name.end());
    const wchar_t* winName = widestr.c_str();

    this->hWnd = CreateWindowEx(0, CLASS_NAME, winName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, this->width,
        this->height, nullptr, nullptr, this->instance, this);

    if (!hWnd) {
        throw std::runtime_error("Failed to create window, error: " + std::to_string(GetLastError()));
    }

    this->Show(nCmdShow);
}

Window::~Window() {
    if (this->hWnd) {
        DestroyWindow(this->hWnd);
    }
    UnregisterClass(L"WINDOW_CLASS", this->instance);
}

HWND Window::GetHWND() const { return this->hWnd; }

UINT Window::GetWidth() const { return this->width; }

UINT Window::GetHeight() const { return this->height; }

void Window::Show(int nCmdShow) {
    ShowWindow(this->hWnd, nCmdShow);
    UpdateWindow(this->hWnd);
    this->UpdateClientSize();
}

void Window::Resize(UINT width, UINT height) {
    if (width == 0 || height == 0) {
        return;
    }
    this->width = width;
    this->height = height;

    if (this->isFullscreen) {
        SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->width, this->height,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_NOMOVE);
        this->UpdateClientSize();

        if (this->resizeCallback) {
            this->resizeCallback(this->width, this->height);
        }
        return;
    }

    SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->width, this->height, SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
}

void Window::ToggleFullscreen(bool fullscreen)
{
    if (this->isFullscreen == fullscreen) {
        return;
    }

    this->isFullscreen = fullscreen;

    if (fullscreen) {
        GetWindowRect(this->hWnd, &this->windowedRect);
        SetWindowLongPtr(this->hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        MONITORINFO mi = { sizeof(mi) };
        if (GetMonitorInfo(MonitorFromWindow(this->hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowPos(this->hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
        this->UpdateClientSize();
        if (this->resizeCallback) {
            this->resizeCallback(this->width, this->height);
        }
    }
    else {
        SetWindowLongPtr(this->hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        SetWindowPos(this->hWnd, HWND_TOP, this->windowedRect.left, this->windowedRect.top,
            this->windowedRect.right - this->windowedRect.left, this->windowedRect.bottom - this->windowedRect.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        this->UpdateClientSize();
        if (this->resizeCallback) {
            this->resizeCallback(this->width, this->height);
        }
    }
}

void Window::SetResizeCallback(std::function<void(UINT, UINT)> callback)
{
    this->resizeCallback = std::move(callback);
}