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
#include "core/physics/rigidBody.h"
#include "core/physics/collision.h"

class SceneManager {
public:
	SceneManager(Renderer* rend); // The renderer reference is temporary
	~SceneManager() = default;

	void SceneTick();

	/// <summary>
	/// For now this serves as a place to build scenes
	/// </summary>
	void LoadScene(); // Should be able to take a scene file?

	//Audio
	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);
	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(std::string id);

private:
	std::unique_ptr<Scene> mainScene;

	Renderer* renderer; // This is temporary

	std::vector<std::unique_ptr<Mesh>> tempMeshes; // This is also temporary
};