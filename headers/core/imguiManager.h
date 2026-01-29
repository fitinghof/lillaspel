#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <Windows.h>
#include <d3d11.h>

class ImguiManager
{
public:
	ImguiManager() = default;
	ImguiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	~ImguiManager();

	void InitalizeImgui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext);

	void imguiAtFrameStart();
	void imguiAtFrameEnd();

private:

};