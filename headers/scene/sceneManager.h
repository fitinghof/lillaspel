#pragma once

#include <memory>
#include <scene/scene.h>
#include <utilities/logger.h>

class SceneManager {
public:
	SceneManager();
	~SceneManager() = default;

	void SceneTick();

	void LoadScene(); // Should be able to take a scene file?

private:
	std::unique_ptr<Scene> mainScene;
};