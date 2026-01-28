#pragma once
#include "utilities/logger.h"
#include "window.h"
#include "rendering/renderer.h"
#include "../headers/core/audio/soundEngine.h"

class Game {
public:
	Game() = default;
	~Game() = default;
	void Run(HINSTANCE hInstance, int nCmdShow);

private:
	AudioManager audioManager;
};