#include "core/imguiManager.h"

ImguiManager::ImguiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	this->InitalizeImgui(hwnd, device, immediateContext);
}

ImguiManager::~ImguiManager() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

ImguiManager::InitalizeImgui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, immediateContext);
}

ImguiManager::imguiAtFrameStart() {
	// (Your code process and dispatch Win32 messages)
// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)
}

ImguiManager::imguiAtFrameEnd() {
	// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls swapchain's Present() function)
}