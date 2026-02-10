#include "core/imguiManager.h"
#include "UI/uiManager.h"
#include "utilities/logger.h"
#include "utilities/time.h"

// std
#include "core/imguiManager.h"
#include <commdlg.h>
#include <format>

namespace {
std::string WideToUtf8(const std::wstring& w) {
	if (w.empty()) return {};
	const int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string out(size - 1, '\0');
	WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, out.data(), size, nullptr, nullptr);
	return out;
}

std::wstring SaveSceneFileDialog(HWND hwnd) {
	wchar_t filePath[MAX_PATH] = L"";

	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Scene Files (*.scene)\0*.scene\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt = L"scene";

	if (GetSaveFileNameW(&ofn)) {
		return std::wstring(filePath);
	}
	return L"";
}

std::wstring OpenSceneFileDialog(HWND hwnd) {
	wchar_t filePath[MAX_PATH] = L"";

	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = filePath;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"Scene Files (*.scene)\0*.scene\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn)) {
		return std::wstring(filePath);
	}
	return L"";
}
} // namespace

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
	io.IniFilename = nullptr;
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

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
	// ImGui::ShowDemoWindow(); // Show demo window! :)
	this->MainMenuImGui();
	this->WindowOptionsImGui();
	this->ConsoleImGui();
	this->UIEditorImGui();
}

void ImguiManager::ImguiAtFrameEnd() {
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls swapchain's Present() function)
}

void ImguiManager::SetResolutionChangeCallback(std::function<void(UINT, UINT)> callback) {
	this->resolutionChangeCallback = std::move(callback);
}

void ImguiManager::SetFullscreenChangeCallback(std::function<void(bool)> callback) {
	this->fullscreenChangeCallback = std::move(callback);
}

void ImguiManager::SetVSyncChangeCallback(std::function<void(bool)> callback) {
	this->vSyncChangeCallback = std::move(callback);
}

void ImguiManager::SetWireframeChangeCallback(std::function<void(bool)> callback) {
	this->wireframeChangeCallback = std::move(callback);
}

void ImguiManager::SetSaveSceneChangeCallback(std::function<void(const std::string&)> callback) {
	this->saveSceneChangeCallback = std::move(callback);
}

void ImguiManager::SetSaveSceneAsChangeCallback(std::function<void(const std::string&)> callback) {
	this->saveSceneAsChangeCallback = std::move(callback);
}
void ImguiManager::SetLoadSceneChangeCallback(std::function<void(const std::string&)> callback) {
	this->loadSceneChangeCallback = std::move(callback);
}

void ImguiManager::ConsoleImGui() {
	if (!this->showConsoleWindow) return;

	ImGui::SetNextWindowSize(ImVec2(300.f, 500.f), ImGuiCond_FirstUseEver);
	ImGui::Begin(std::format("console  |  AVG FPS:{} ### consoleImGuiWindow", this->GetAverageFrameRate()).c_str(),
				 &this->showConsoleWindow, ImGuiWindowFlags_MenuBar);

	ImGui::BeginChild("console output");
	const auto& log = Logger::getLogStringRef();

	static std::vector<char> logBuffer;
	if (logBuffer.size() < log.size() + 1) {
		logBuffer.resize(log.size() + 1);
	}

	std::memcpy(logBuffer.data(), log.data(), log.size());
	logBuffer[log.size()] = '\0';

	ImGui::InputTextMultiline("##console_text", logBuffer.data(), logBuffer.size(), ImVec2(-FLT_MIN, -FLT_MIN),
							  ImGuiInputTextFlags_ReadOnly);

	bool scrollToBottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY());
	if (scrollToBottom) {
		ImGui::SetScrollHereY(1.0f);
	}

	ImGui::EndChild();
	ImGui::End();
}

void ImguiManager::WindowOptionsImGui() {
	if (!this->showOptionsWindow) return;

	struct ResolutionPreset {
		const char* label;
		UINT width;
		UINT height;
	};

	static const ResolutionPreset presets[] = {
		{"800x600", 800, 600},	   {"1024x576", 1024, 576},	  {"1280x720", 1280, 720},
		{"1920x1080", 1920, 1080}, {"2560x1440", 2560, 1440}, {"3840x2160", 1920, 1080} // illusion of choice
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
	bool vSyncChanged = ImGui::Checkbox("V-Sync", &this->isVSync);

	if (selectionChanged && this->resolutionChangeCallback) {
		this->resolutionChangeCallback(presets[selectedPreset].width, presets[selectedPreset].height);
	}

	if (fullscreenChanged && this->fullscreenChangeCallback) {
		this->fullscreenChangeCallback(this->isFullscreen);
	}

	if (vSyncChanged && this->vSyncChangeCallback) {
		this->vSyncChangeCallback(this->isVSync);
	}

	ImGui::Text("Selected: %ux%u", presets[selectedPreset].width, presets[selectedPreset].height);
	ImGui::End();
}

void ImguiManager::MainMenuImGui() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Options")) {
			ImGui::MenuItem("Window Options", nullptr, &this->showOptionsWindow);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug")) {
			ImGui::MenuItem("Console", nullptr, &this->showConsoleWindow);
			ImGui::MenuItem("Wireframe", nullptr, &this->showWireframe);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::MenuItem("UI", nullptr, &this->showUIEditorWindow);
			ImGui::MenuItem("Transform", nullptr, &this->showTransformWindow);
			ImGui::MenuItem("Sound", nullptr, &this->showSoundWindow);
			ImGui::MenuItem("Music", nullptr, &this->showMusicWindow);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene")) {
			if (ImGui::MenuItem("Save") && this->saveSceneChangeCallback) {
				if (this->currentScenePath.empty()) {
					const std::wstring pathW = SaveSceneFileDialog(GetActiveWindow());
					if (!pathW.empty()) {
						this->currentScenePath = WideToUtf8(pathW);
						this->saveSceneChangeCallback(this->currentScenePath);
					}
				} else {
					this->saveSceneChangeCallback(this->currentScenePath);
				}
			}

			if (ImGui::MenuItem("Save As...") && this->saveSceneAsChangeCallback) {
				const std::wstring pathW = SaveSceneFileDialog(GetActiveWindow());
				if (!pathW.empty()) {
					this->currentScenePath = WideToUtf8(pathW);
					this->saveSceneAsChangeCallback(this->currentScenePath);
				}
			}

			if (ImGui::MenuItem("Load") && this->loadSceneChangeCallback) {
				const std::wstring pathW = OpenSceneFileDialog(GetActiveWindow());
				if (!pathW.empty()) {
					this->currentScenePath = WideToUtf8(pathW);
					this->loadSceneChangeCallback(this->currentScenePath);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImguiManager::UIEditorImGui() {
	if (!this->showUIEditorWindow) return;
	ImGui::Begin("UI Editor", &this->showUIEditorWindow);

	// Add Canvas
	if (ImGui::Button("Add Canvas") && UI::uiManager) {
		auto canvas = std::make_shared<UI::Canvas>();
		canvas->SetSize({800, 600});
		UI::uiManager->AddCanvas(canvas);
	}

	// List canvases
	if (!UI::uiManager) {
		Logger::Error("UI Manager failed to link with imgui!");
	} else {
		const auto& canvases = UI::uiManager->GetCanvases();
		int canvasIndex = 0;
		for (const auto& canvas : canvases) {
			ImGui::PushID(canvasIndex);
			if (ImGui::TreeNode(std::format("Canvas {}", canvasIndex).c_str())) {
				UI::Vec2 size = canvas->GetSize();
				if (ImGui::InputFloat2("Size", &size.x)) {
					canvas->SetSize(size);
				}

				// Add button
				if (ImGui::Button("Add Button")) {
					auto btn = std::make_shared<UI::Button>();
					btn->SetPosition({100, 100});
					btn->SetSize({200, 50});
					canvas->AddChild(btn);
				}

				// List widgets
				int widgetIndex = 0;
				const auto children = canvas->GetChildren();
				for (const auto& widget : children) {
					ImGui::PushID(widgetIndex);
					if (ImGui::TreeNode(std::format("Widget {}", widgetIndex).c_str())) {
						UI::Vec2 pos = widget->GetPosition();
						UI::Vec2 size = widget->GetSize();
						if (ImGui::InputFloat2("Position", &pos.x)) {
							widget->SetPosition(pos);
						}
						if (ImGui::InputFloat2("Size", &size.x)) {
							widget->SetSize(size);
						}
						ImGui::TreePop();
					}
					ImGui::PopID();
					++widgetIndex;
				}

				ImGui::TreePop();
			}
			ImGui::PopID();
			++canvasIndex;
		}
	}

	ImGui::End();
}

// not great to hardcode time to avrage fps over
float ImguiManager::GetAverageFrameRate() {
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
