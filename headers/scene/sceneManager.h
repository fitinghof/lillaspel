#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <scene/scene.h>
#include <utilities/logger.h>
#include "gameObjects/cameraObject.h"
#include "core/assetManager.h"
#include "gameObjects/meshObject.h"
#include "rendering/renderer.h"
#include "scene/objectFromStringFactory.h"
#include "gameObjects/debugCamera.h"
#include "core/audio/soundEngine.h"

#include <fstream>
#include <nlohmann/json.hpp>

class SceneManager
{
public:
	SceneManager(Renderer *rend); // The renderer reference is temporary
	~SceneManager() = default;

	void SceneTick();

	/// <summary>
	/// For now this serves as a place to build scenes
	/// </summary>
	void LoadScene();

	//Sound effects
	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);
	std::string GetPathToSoundFolder();
	SoundClip *GetSoundClip(std::string id);

	//Music
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
	void SetMasterMusicGain(float gain);
	MusicTrack* GetMusicTrack(std::string id);

	void LoadSceneFromFile(const std::string& filePath);
	void CreateObjectsFromJsonRecursively(const nlohmann::json& data, std::weak_ptr<GameObject> parent);
	void SaveSceneToFile(const std::string& filePath);

private:
	std::unique_ptr<Scene> mainScene;
	ObjectFromStringFactory objectFromString;
	AudioManager audioManager;

	Renderer *renderer; // This is temporary
	std::string currentScenePath;

	std::vector<std::unique_ptr<Mesh>> tempMeshes; // This is also temporary
};