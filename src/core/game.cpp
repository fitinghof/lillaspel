#include "core/game.h"
#include "core/imguiManager.h"
#include "scene/sceneManager.h"
#include "utilities/time.h"
#include <memory>
#include "core/inputManager.h"
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

    float timer1 = 10.0f;
    float timer2 = 2.0f;

    //Play music example
    this->audioManager.InitializeMusicTrackManager("../../assets/audio/music/");
    this->audioManager.AddMusicTrackStandardFolder("Sneak16.wav", "sneak");
    this->audioManager.SetMusicGain(1.0f);
    this->audioManager.SetGain("sneak", 0.1f);
    this->audioManager.SetPitch("sneak", 1.05f);
    this->audioManager.FadeInPlay("sneak", 0, 6.0f); //un-comment to listen

    //Play 3D sound effect example
    SetListenerPosition(3, 0, -1);
    srand(0);

    this->sceneManager.get()->InitializeSoundBank("../../assets/audio/soundeffects/");
    this->sceneManager.get()->AddSoundClipStandardFolder("Step.wav", "step");
    SoundClip* soundClip = sceneManager.get()->GetSoundClip("step");

    SoundSourceObject speaker;
    speaker.transform.SetPosition(DirectX::XMVectorZero());
    speaker.SetGain(0.7f);

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

        this->audioManager.Tick();
        speaker.Tick(); //remember to tick speakers, as with any game object

        //fade out music example
        timer1 -= Time::GetInstance().GetDeltaTime();
        timer2 -= Time::GetInstance().GetDeltaTime();
        if (timer1 <= 0)
        {
            //should be called once
            this->audioManager.FadeOutStop("sneak", 8.0f);
            timer1 = 10;
        }

        //randomize position and pitch example
        if (timer2 <= 0)
        {
            DirectX::XMVECTOR pos;
            pos.m128_f32[0] = rand() % 16 - 8;
            pos.m128_f32[1] = rand() % 16 - 8;
            pos.m128_f32[2] = rand() % 16 - 8;

            speaker.transform.SetPosition(pos);
            speaker.SetRandomPitch(0.8f, 1.2f);
            speaker.Play(soundClip); //un-comment to listen

            timer2 = 2;
        }
    }
}

void Game::InitializeSoundBank(std::string pathToSoundFolder)
{
    this->sceneManager.get()->InitializeSoundBank(pathToSoundFolder);
}

void Game::AddSoundClipStandardFolder(std::string filename, std::string id)
{
    this->sceneManager.get()->AddSoundClipStandardFolder(filename, id);
}

void Game::AddSoundClip(std::string path, std::string id)
{
    this->sceneManager.get()->AddSoundClip(path, id);
}

std::string Game::GetPathToSoundFolder()
{
    return this->sceneManager.get()->GetPathToSoundFolder();
}

SoundClip* Game::GetSoundClip(std::string id)
{
    return this->sceneManager.get()->GetSoundClip(id);
}

void Game::AudioManagerTick()
{
    this->audioManager.Tick();
}

void Game::InitializeMusicTrackManager(std::string pathToMusicFolder)
{
    this->audioManager.InitializeMusicTrackManager(pathToMusicFolder);
}

void Game::AddMusicTrackStandardFolder(std::string filename, std::string id)
{
    this->audioManager.AddMusicTrackStandardFolder(filename, id);
}

void Game::AddMusicTrack(std::string path, std::string id)
{
    this->audioManager.AddMusicTrack(path, id);
}

void Game::PlayMusicTrack(std::string id)
{
    this->audioManager.Play(id);
}

void Game::StopMusicTrack(std::string id)
{
    this->audioManager.Stop(id);
}

void Game::FadeInPlayMusicTrack(std::string id, float startGain, float seconds)
{
    this->audioManager.FadeInPlay(id, startGain, seconds);
}

void Game::FadeOutStopMusicTrack(std::string id, float seconds)
{
    this->audioManager.FadeOutStop(id, seconds);
}

void Game::GetMusicTrackSourceState(std::string id, ALint& sourceState)
{
    this->audioManager.GetMusicTrackSourceState(id, sourceState);
}

void Game::SetMusicTrackGain(std::string id, float gain)
{
    this->audioManager.SetGain(id, gain);
}

MusicTrack* Game::GetMusicTrack(std::string id)
{
    return this->audioManager.GetMusicTrack(id);
}
