#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <memory>
#include <scene/scene.h>
#include <utilities/logger.h>
#include "gameObjects/cameraObject.h"
#include "gameObjects/meshObject.h"
#include "rendering/renderer.h"

class SceneManager {
public:
	SceneManager(Renderer* rend);
	~SceneManager() = default;

	void SceneTick();

	void LoadScene(); // Should be able to take a scene file?

private:
	std::unique_ptr<Scene> mainScene;

	Renderer* renderer; // This is temporary

	std::unique_ptr<Mesh> tempMesh; // This is also temporary
};