#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "core/window.h"
#include "rendering/renderer.h"
#include "utilities/logger.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "UI/uiManager.h"
#include "core/imguiManager.h"
#include "scene/sceneManager.h"

class Game {
public:
	Game() = default;
	~Game() = default;
	void Run(HINSTANCE hInstance, int nCmdShow);

	UI::UIManager* GetUIManager();

	// audio manager has been moved to scene manager for better access

private:
	ImguiManager imguiManager;
	Renderer renderer;
	std::unique_ptr<SceneManager> sceneManager;
	std::unique_ptr<UI::UIManager> uiManager;
};