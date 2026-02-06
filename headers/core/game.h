#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "utilities/logger.h"
#include "core/window.h"
#include "rendering/renderer.h"
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include "core/imguiManager.h"
#include "core/audio/soundEngine.h"
#include "scene/sceneManager.h"

class Game {
public:
	Game() = default;
	~Game() = default;
	void Run(HINSTANCE hInstance, int nCmdShow);

	//Audio, sound effects
	void InitializeSoundBank(std::string pathToSoundFolder); //always end path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);
	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(std::string id);

	//Audio, music
	void AudioManagerTick();
	void InitializeMusicTrackManager(std::string pathToMusicFolder); // always end path with /
	void AddMusicTrackStandardFolder(std::string filename, std::string id);
	void AddMusicTrack(std::string path, std::string id);
	void PlayMusicTrack(std::string id);
	void StopMusicTrack(std::string id);
	void FadeInPlayMusicTrack(std::string id, float startGain, float seconds);
	void FadeOutStopMusicTrack(std::string id, float seconds);
	void GetMusicTrackSourceState(std::string id, ALint& sourceState);
	void SetMusicTrackGain(std::string id, float gain);
	MusicTrack* GetMusicTrack(std::string id);

private:
	ImguiManager imguiManager;
	Renderer renderer;
	AudioManager audioManager;
	std::unique_ptr<SceneManager> sceneManager;
};