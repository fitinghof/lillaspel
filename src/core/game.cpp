#include "core/game.h"

// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
	Window window(hInstance, nCmdShow, "Game Window");

    Renderer renderer;
    renderer.Init(window);

	MSG msg = {};

    SoundBank soundBank;
    soundBank.Initialize("C:/Users/Gabriel/Music/");
    soundBank.AddSoundClipStandardFolder("Electro4.wav");

    SoundClip* clip = soundBank.GetSoundClipStandardFolder("Electro4.wav");

    SoundSource speaker;
    speaker.Play(clip);
    speaker.SetPosition(20, 0, 1);

    SetListenerPosition(0, 0, 0);

    MusicTrack music;
    music.Initialize("C:/Users/Gabriel/Music/");
    music.LoadTrackStandardFolder("Electro4.wav");
    music.SetPitch(0.94f);
    //music.Play();

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        music.UpdateBufferStream();
    }
}