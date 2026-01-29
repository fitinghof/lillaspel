#include "scene/sceneManager.h"

SceneManager::SceneManager() : mainScene(nullptr)
{
}

void SceneManager::SceneTick()
{
	if (!this->mainScene) return;

	this->mainScene->SceneTick();
}

void SceneManager::LoadScene()
{
	if (this->mainScene) {
		// TO DO: Delete old scene

		throw std::runtime_error("Tried to load another scene, unsupported.");
	}

	this->mainScene = std::unique_ptr<Scene>(new Scene());

	CameraObject* camera = (CameraObject*)this->mainScene->CreateGameObjectOfType<CameraObject>();
}
