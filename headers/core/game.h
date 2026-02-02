#pragma once
#include "utilities/logger.h"
#include "window.h"
#include "rendering/renderer.h"
#include "core/imguiManager.h"
#include "core/audio/soundEngine.h"
#include "scene/sceneManager.h"

class Game {
public:
	Game() = default;
	~Game() = default;
	void Run(HINSTANCE hInstance, int nCmdShow);

private:
	ImguiManager imguiManager;
	Renderer renderer;
	AudioManager audioManager;
	SceneManager sceneManager;
};