#include "core/imguiManager.h"
#include "utilities/logger.h"
#include "utilities/time.h"
#include <format>
ImguiManager::ImguiManager(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	this->InitalizeImgui(hwnd, device, immediateContext);
}

ImguiManager::~ImguiManager() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::InitalizeImgui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, immediateContext);
}

void ImguiManager::ImguiAtFrameStart() {
	// (Your code process and dispatch Win32 messages)
// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // Show demo window! :)
	this->ConsoleImGui();
}

void ImguiManager::ImguiAtFrameEnd() {
	// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls swapchain's Present() function)
}

void ImguiManager::ConsoleImGui()
{
	static bool isOpen = true;
	if (!isOpen)return;

	ImGui::SetNextWindowSize(ImVec2(300.f, 500.f), ImGuiCond_FirstUseEver);
	ImGui::Begin(std::format("console  |  AVG FPS:{} ### consoleImGuiWindow", this->GetAvrageFrameRate()).c_str(), &isOpen);
	ImGui::BeginChild("console output");
	ImGui::TextWrapped(Logger::getLogStringRef().data());
	bool scrollToBottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY());
	if (scrollToBottom) {
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();
	ImGui::End();
}
//not great to hardcode time to avrage fps over
float ImguiManager::GetAvrageFrameRate()
{
	static int frameCount = 0.0f;
	static float timeAtLastFPSCalc = 0.0f;
	static float FPS = 0.0f;
	frameCount++;
	float timeSinceLastFPSCalc = Time::GetInstance().GetSessionTime() - timeAtLastFPSCalc;
	if (timeSinceLastFPSCalc >= 0.5f) {
		timeAtLastFPSCalc = Time::GetInstance().GetSessionTime();
		FPS = frameCount / timeSinceLastFPSCalc;
		frameCount = 0;
	}
	return FPS;
}
