#include "scene/scene.h"

Scene::Scene() : gameObjects()
{
}

void Scene::SceneTick()
{
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::shared_ptr<GameObject> gameObject = this->gameObjects[i];
		gameObject->Tick();
		gameObject->LateTick();
	}

	this->DeleteDeleteQueue();
}

void Scene::RegisterGameObject(std::shared_ptr<GameObject> gameObject)
{
	this->gameObjects.push_back(gameObject);
	gameObject->factory = this;
	gameObject->myPtr = gameObject;
	gameObject->Start();
}

void Scene::QueueDeleteGameObject(std::weak_ptr<GameObject> gameObject)
{
	this->deleteQueue.push_back(gameObject);
}

size_t Scene::GetNumberOfGameObjects()
{
	return this->gameObjects.size();
}

void Scene::DeleteDeleteQueue()
{
	if (deleteQueue.empty()) {
		return;
	}

	if (gameObjects.empty()) {
		Logger::Warn("Tried to delete at least one GameObject, but there are no GameObjects in the scene. Aborted.");
		return;
	}

	for (auto gameObject : this->deleteQueue) {
		auto iterator = std::find(this->gameObjects.begin(), this->gameObjects.end(), gameObject.lock());

		if (iterator == this->gameObjects.end()) {
			Logger::Warn("Tried to delete a GameObject that is not present in the scene.");
		}
		else {
			this->gameObjects.erase(iterator);
		}
	}

	this->deleteQueue.clear();
}
