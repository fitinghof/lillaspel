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
    this->sceneManager->LoadScene();

    MSG msg = {};

    MusicTrack track1;
    track1.Initialize("C:/Users/Gabriel/Music/Testing/");
    track1.LoadTrackStandardFolder("Sneak16.wav", "sneak");
    track1.Play();

    track1.SetGain(0.5f);

    MusicTrack track2;
    track2.Initialize("C:/Users/Gabriel/Music/Testing/");
    track2.LoadTrackStandardFolder("BloodyFangs16.wav", "fangs");
    track2.Play();


    while (msg.message != WM_QUIT)
    {
        this->imguiManager.ImguiAtFrameStart();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        track1.UpdateBufferStream();
        track2.UpdateBufferStream();

        Time::GetInstance().Tick();
        this->sceneManager->SceneTick();

        this->renderer.Render();
        this->imguiManager.ImguiAtFrameEnd();
        this->renderer.Present();
    }
}