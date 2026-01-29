#include "core/game.h"
#include "core/imguiManager.h"
// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
    Window window(hInstance, nCmdShow, "Game Window");

    Renderer renderer;
    renderer.Init(window);

    this->imguiManager.InitalizeImgui(window.GetHWND(), renderer.GetDevice(), renderer.GetContext());

    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        this->imguiManager.imguiAtFrameStart();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        renderer.Render();
        this->imguiManager.imguiAtFrameEnd();
        renderer.Present();
    }
}