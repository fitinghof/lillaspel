#include "core/window.h"
#include "imgui.h"
#include "utilities/logger.h"

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

    if (this->showIMGui) {
		ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
    }

	return this->ReadMessage(hWnd, message, wParam, lParam);
}

Window::Window(const HINSTANCE instance, int nCmdShow, const std::string name, const UINT width, const UINT height)
    : instance(instance), width(width), height(height), hWnd(nullptr), isFullscreen(false), showIMGui(true), cursorVisible(true), inputManager(std::make_unique<InputManager>()) {

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

LRESULT Window::ReadMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const unsigned char key = static_cast<unsigned char>(wParam);
	switch (message) {

	/// Handle keyboard events
	case WM_KEYDOWN: {
		switch (key) {
			case VK_ESCAPE: {
				PostQuitMessage(0);
				break;
			}
			case VK_F11: {
				//window->SetFullscreen(!window->IsFullscreen());
				break;
			}
			case VK_TAB: {
				// So far does nothing with the actual IMGui window, but is set up for toggling
				this->showIMGui = !this->showIMGui;
				Logger::Log("Toggling IMGui " + std::string(this->showIMGui ? "on" : "off"));

				// Can also be set to its own keybind
				this->cursorVisible = !this->cursorVisible;
				ShowCursor(this->cursorVisible);
				break;
			}

			default: {
				const bool wasDown = lParam & (1 << 30);
				if (!wasDown)
					this->inputManager->SetKeyState(key, KEY_DOWN | KEY_PRESSED);
				break;
			}
		}
		return 0;
	}

	case WM_KEYUP: {
		this->inputManager->SetKeyState(key, KEY_RELEASED);
		return 0;
	}

	/// Handle mouse events
	case WM_MOUSEMOVE: {
		const int xPos = GET_X_LPARAM(lParam);
		const int yPos = GET_Y_LPARAM(lParam);
		this->inputManager->SetMousePosition(xPos, yPos);
		return 0;
	}

	case WM_LBUTTONDOWN: {
		if (!this->inputManager->IsLMDown())
			this->inputManager->SetLMouseKeyState(KEY_DOWN | KEY_PRESSED);
		return 0;
	}

	case WM_LBUTTONUP: {
		this->inputManager->SetLMouseKeyState(KEY_RELEASED);
		return 0;
	}

	case WM_RBUTTONDOWN: {
		if (!this->inputManager->IsRMDown())
			this->inputManager->SetRMouseKeyState(KEY_DOWN | KEY_PRESSED);
		return 0;
	}

	case WM_RBUTTONUP: {
		this->inputManager->SetRMouseKeyState(KEY_RELEASED);
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

HWND Window::GetHWND() const { return this->hWnd; }

UINT Window::GetWidth() const { return this->width; }

UINT Window::GetHeight() const { return this->height; }

InputManager* Window::GetInputManager() const { return this->inputManager.get(); }

bool Window::IsFullscreen() const { return this->isFullscreen; }

void Window::Show(int nCmdShow) const {
    ShowWindow(this->hWnd, nCmdShow);
    UpdateWindow(this->hWnd);
}

bool Window::IsIMGuiShown() const { return this->showIMGui; }

void Window::SetIMGuiShown(const bool show) { this->showIMGui = show; }

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
//    this->isFullscreen = fullscreen;
//}
