#pragma once

#include <memory>
#include <scene/scene.h>
#include <utilities/logger.h>
#include "gameObjects/cameraObject.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager() = default;

	void SceneTick();

	void LoadScene(); // Should be able to take a scene file?

private:
	std::unique_ptr<Scene> mainScene;
};