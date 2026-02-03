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

    MusicTrackManager mtm;
    mtm.Initialize("C:/Users/Gabriel/Music/Testing/");
    mtm.AddMusicTrackStandardFolder("Sneak16.wav", "sneak");
    mtm.AddMusicTrackStandardFolder("BloodyFangs16.wav", "fangs");

    mtm.SetGain("sneak", 0.8f);
    mtm.FadeInPlay("sneak", 0, 12);

    SoundBank sb;
    sb.Initialize("C:/Users/Gabriel/Music/Testing/");
    sb.AddSoundClipStandardFolder("TestSound.wav", "scream1");

    SoundClip* clip = sb.GetSoundClip("scream1");

    SoundSourceObject sso;
    sso.SetRandomPitch(1.2f, 1.8f);

    float time = 3;
    float timer = time;

    SetListenerPosition(-5, 0, -1);


    while (msg.message != WM_QUIT)
    {
        this->imguiManager.ImguiAtFrameStart();
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        timer -= Time::GetInstance().GetDeltaTime();

        mtm.Tick();

        if (timer <= 0)
        {
            timer = time;
            //mtm.Stop("fangs");
            sso.Play(clip);
        }

        Time::GetInstance().Tick();
        this->sceneManager->SceneTick();

        this->renderer.Render();
        this->imguiManager.ImguiAtFrameEnd();
        this->renderer.Present();
    }
}