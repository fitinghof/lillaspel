#include "core/game.h"
#include "core/imguiManager.h"
#include "scene/sceneManager.h"
#include <memory>
// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
    Window window(hInstance, nCmdShow, "Game Window");

    this->renderer.Init(window);

    this->imguiManager.InitalizeImgui(window.GetHWND(), this->renderer.GetDevice(), this->renderer.GetContext());
    this->sceneManager.LoadScene();

    MSG msg = {};


    while (msg.message != WM_QUIT)
    {
        this->imguiManager.imguiAtFrameStart();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        this->renderer.Render();
        this->imguiManager.imguiAtFrameEnd();
        this->renderer.Present();
    }
}