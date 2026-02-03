#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>
#include <d3d11.h>
#include <functional>

class ImguiManager
{
public:
	ImguiManager() = default;
	ImguiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~ImguiManager();

	void InitalizeImgui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	void ImguiAtFrameStart();
	void ImguiAtFrameEnd();

	void SetResolutionChangeCallback(std::function<void(UINT, UINT)> callback);
	void SetFullscreenChangeCallback(std::function<void(bool)> callback);

private:

	void ConsoleImGui();
	void OptionsImGui();
	void MainMenuImGui();
	float GetAvrageFrameRate();

	bool showOptionsWindow = false;
	bool showConsoleWindow = false;
	bool isFullscreen = false;

	std::function<void(UINT, UINT)> resolutionChangeCallback;
	std::function<void(bool)> fullscreenChangeCallback;

};