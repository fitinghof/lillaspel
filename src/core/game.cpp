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

    this->sceneManager.assetManager.InitializeSoundBank("C:/Users/Gabriel/Music/");
    this->sceneManager.assetManager.AddSoundClipStandardFolder("TestSound.wav");

    SoundClip* clip = this->sceneManager.assetManager.GetSoundClipStandardFolder("TestSound.wav");
    SetListenerPosition(-3, 0, 0);
    this->sceneManager.assetManager.soundSource.SetPosition(-3, 0, 1);
    this->sceneManager.assetManager.soundSource.Play(clip);

    //SoundSource ss;
    //ss.SetPosition(-3, 0, 1);
    //ss.Play(clip);

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