#include "core/game.h"
#include "core/imguiManager.h"
#include "scene/sceneManager.h"
#include "utilities/time.h"
#include <memory>
// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
    Window window(hInstance, nCmdShow, "Game Window");

    Renderer renderer;
    renderer.Init(window);

    this->imguiManager.InitalizeImgui(window.GetHWND(), renderer.GetDevice(), renderer.GetContext());

    std::unique_ptr<SceneManager> sceneMan = std::unique_ptr<SceneManager>(new SceneManager());
    sceneMan->LoadScene();

    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        this->imguiManager.ImguiAtFrameStart();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Time::GetInstance().Tick();
        sceneMan->SceneTick();
        renderer.Render();
        this->imguiManager.ImguiAtFrameEnd();
        renderer.Present();
    }
}