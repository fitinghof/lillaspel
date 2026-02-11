#include "scene/scene.h"

Scene::Scene() : gameObjects(), finishedLoading(true) {
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
	if (this->finishedLoading) {
		gameObject->Start();
	}
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
	if (this->deleteQueue.empty()) {
		return;
	}

	Logger::Log("GameObject count ", this->gameObjects.size());

	if (this->gameObjects.empty()) {
		Logger::Warn("Tried to delete at least one GameObject, but there are no GameObjects in the scene. Aborted.");
		return;
	}

	for (size_t i = 0; i < this->deleteQueue.size(); i++) {
		if (this->deleteQueue[i].expired()) continue; // If object is already destroyed

		auto gameObject = this->deleteQueue[i].lock();

		auto iterator = std::find(this->gameObjects.begin(), this->gameObjects.end(), gameObject);

		if (iterator == this->gameObjects.end()) {
			Logger::Warn("Tried to delete a GameObject that is not present in the scene.");
			continue;
		}
		
		if (gameObject->GetChildCount() > 0) {
			for (auto child : gameObject->children) {
				this->deleteQueue.push_back(child);
			}
		}

		gameObject->OnDestroy();
		this->gameObjects.erase(iterator); // Actually deletes
	}

	this->deleteQueue.clear();

	Logger::Log("GameObject count ", this->gameObjects.size());
}

void Scene::CallStartOnAll() {
	for (size_t i = 0; i < this->gameObjects.size(); i++) {
		std::shared_ptr<GameObject> gameObject = this->gameObjects[i];
		gameObject->Start();
	}
}

const std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects() const { return this->gameObjects; }
