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

#include <fstream>
#include <nlohmann/json.hpp>

class SceneManager {
public:
	SceneManager(Renderer* rend); // The renderer reference is temporary
	~SceneManager() = default;

	void SceneTick();

	/// <summary>
	/// For now this serves as a place to build scenes
	/// </summary>
	void LoadScene(); 

	//Audio
	void InitializeSoundBank(std::string pathToSoundFolder); //end the path with /
	void AddSoundClipStandardFolder(std::string filename, std::string id);
	void AddSoundClip(std::string path, std::string id);
	std::string GetPathToSoundFolder();
	SoundClip* GetSoundClip(std::string id);

	void CreateNewScene(std::shared_ptr<Scene>& scene);
	void DeleteScene(std::shared_ptr<Scene>& scene);
	void LoadSceneFromFile(const std::string& filePath);
	void CreateObjectsFromJsonRecursively(const nlohmann::json& data, std::weak_ptr<GameObject> parent);
	void SaveSceneToFile(const std::string& filePath);

private:
	std::shared_ptr<Scene> mainScene;
	std::shared_ptr<Scene> emptyScene;
	ObjectFromStringFactory objectFromString;

	Renderer* renderer; // This is temporary

	std::vector<std::unique_ptr<Mesh>> tempMeshes; // This is also temporary
};