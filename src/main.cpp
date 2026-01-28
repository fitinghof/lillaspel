#include "Windows.h"

#include <iostream>
#include "test.h"
#include "utilities/logger.h"
#include "core/game.h"

// Main code
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) 
{
	// Maybe put some memory leak flags here

	std::unique_ptr<Game> game = std::unique_ptr<Game>(new Game());

	try {
		game->Run(hInstance, nCmdShow);
	}
	catch (const std::exception& e) {
		Logger::Error(e.what());
	}

	Logger::Log("Exiting game...");
}
