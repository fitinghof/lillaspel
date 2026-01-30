#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "utilities/logger.h"
#include "core/window.h"
#include "rendering/renderer.h"
#include "core/imguiManager.h"

class Game {
public:
	Game() = default;
	~Game() = default;
	void Run(HINSTANCE hInstance, int nCmdShow);

private:

	ImguiManager imguiManager;
};