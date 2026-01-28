#include "core/game.h"

// Game Loop
void Game::Run(HINSTANCE hInstance, int nCmdShow) {
	Window window(hInstance, nCmdShow, "Game Window");

    Renderer renderer;
    renderer.Init(window);

	MSG msg = {};

    SoundBank soundBank;
    soundBank.Initialize("C:/Users/Gabriel/MinaFiler/lillaspel/src/core/audio/");
    soundBank.AddSoundClipStandardFolder("JazzAnger.wav");



    SoundSource speaker;
    speaker.Play(soundBank.GetSoundClipStandardFolder("JazzAnger.wav"));

    MusicTrack music;
    music.Initialize("C:/Users/Gabriel/MinaFiler/lillaspel/src/core/audio/");
    music.LoadTrackStandardFolder("JazzAnger.wav");
    music.SetPitch(0.94f);
    music.Play();

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