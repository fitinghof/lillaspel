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
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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

void Window::Show(int nCmdShow) const {
    ShowWindow(this->hWnd, nCmdShow);
    UpdateWindow(this->hWnd);
}

//void Window::SetFullscreen(bool fullscreen) const {
//    if (fullscreen) {
//        SetWindowLongPtr(this->hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
//        MONITORINFO mi = { sizeof(mi) };
//        if (GetMonitorInfo(MonitorFromWindow(this->hWnd, MONITOR_DEFAULTTOPRIMARY), &mi)) {
//            SetWindowPos(this->hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
//                mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top,
//                SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
//        }
//    }
//    else {
//        SetWindowLongPtr(this->hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
//        SetWindowPos(this->hWnd, HWND_TOP, 0, 0, this->width, this->height, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
//    }
//}
