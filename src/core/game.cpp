#include "core/game.h"

// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
	Window window(hInstance, nCmdShow, "Game Window");

    Renderer renderer;
    renderer.Init(window);
    

	MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer.Render();

        renderer.Present();
    }
}