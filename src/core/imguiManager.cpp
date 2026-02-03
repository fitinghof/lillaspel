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
	this->MainMenuImGui();
	this->OptionsImGui();
	this->ConsoleImGui();
}

void ImguiManager::ImguiAtFrameEnd() {
	// Rendering
// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls swapchain's Present() function)
}

void ImguiManager::SetResolutionChangeCallback(std::function<void(UINT, UINT)> callback)
{
	this->resolutionChangeCallback = std::move(callback);
}

void ImguiManager::SetFullscreenChangeCallback(std::function<void(bool)> callback)
{
	this->fullscreenChangeCallback = std::move(callback);
}

void ImguiManager::ConsoleImGui()
{
	static bool isOpen = true;
	if (!this->showConsoleWindow) return;

	ImGui::SetNextWindowSize(ImVec2(300.f, 500.f), ImGuiCond_FirstUseEver);
	ImGui::Begin(std::format("console  |  AVG FPS:{} ### consoleImGuiWindow", this->GetAvrageFrameRate()).c_str(), &this->showConsoleWindow);
	ImGui::BeginChild("console output");
	ImGui::TextWrapped(Logger::getLogStringRef().data());
	bool scrollToBottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY());
	if (scrollToBottom) {
		ImGui::SetScrollHereY(1.0f);
	}
	ImGui::EndChild();
	ImGui::End();
}

void ImguiManager::OptionsImGui()
{
	if (!this->showOptionsWindow) return;

	struct ResolutionPreset {
		const char* label;
		UINT width;
		UINT height;
	};

	static const ResolutionPreset presets[] = {
		{"800x600", 800, 600},
		{"1024x576", 1024, 576},
		{"1280x720", 1280, 720},
		{"1920x1080", 1920, 1080},
		{"2560x1440", 2560, 1440},
		{"3840x2160", 1920, 1080} // illusion of choice
	};

	static int selectedPreset = 0;
	bool selectionChanged = false;

	ImGui::SetNextWindowSize(ImVec2(300.f, 200.f), ImGuiCond_FirstUseEver);
	ImGui::Begin("options ### optionsImGuiWindow", &this->showOptionsWindow);

	if (ImGui::BeginCombo("Resolution", presets[selectedPreset].label)) {
		for (int i = 0; i < IM_ARRAYSIZE(presets); i++) {
			bool isSelected = (selectedPreset == i);
			if (ImGui::Selectable(presets[i].label, isSelected)) {
				selectedPreset = i;
				selectionChanged = true;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	bool fullscreenChanged = ImGui::Checkbox("Fullscreen", &this->isFullscreen);

	if(selectionChanged && this->resolutionChangeCallback) {
		this->resolutionChangeCallback(presets[selectedPreset].width, presets[selectedPreset].height);
	}

	if (fullscreenChanged && this->fullscreenChangeCallback) {
		this->fullscreenChangeCallback(this->isFullscreen);
	}

	ImGui::Text("Selected: %ux%u", presets[selectedPreset].width, presets[selectedPreset].height);
	ImGui::End();
}

void ImguiManager::MainMenuImGui()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			ImGui::MenuItem("Show Options Window", nullptr, &this->showOptionsWindow);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Console")) {
			ImGui::MenuItem("Show Console Window", nullptr, &this->showConsoleWindow);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
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
