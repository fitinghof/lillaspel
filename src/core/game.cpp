#include "core/game.h"
#include "core/imguiManager.h"
#include "scene/sceneManager.h"
#include "utilities/time.h"
#include <memory>
// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
    Window window(hInstance, nCmdShow, "Game Window");

    this->renderer.Init(window);
    this->sceneManager = std::make_unique<SceneManager>(&renderer);

    this->imguiManager.InitalizeImgui(window.GetHWND(), this->renderer.GetDevice(), this->renderer.GetContext());
    this->imguiManager.SetResolutionChangeCallback([&](UINT width, UINT height) { window.Resize(width, height); });
	this->imguiManager.SetFullscreenChangeCallback([&](bool fullscreen) { window.ToggleFullscreen(fullscreen); });
    window.SetResizeCallback([&](UINT, UINT) { this->renderer.Resize(window); });
    this->imguiManager.SetVSyncChangeCallback([&](bool enable) { this->renderer.ToggleVSync(enable); });

    this->sceneManager->LoadScene();

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
        this->sceneManager->SceneTick();

        this->renderer.Render();
        this->imguiManager.ImguiAtFrameEnd();
        this->renderer.Present();
    }
}